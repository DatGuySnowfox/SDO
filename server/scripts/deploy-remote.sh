#!/usr/bin/env bash
#
# Deploy the dedicated server to a remote Docker host over SSH.
#
#   server/scripts/deploy-remote.sh <ssh-host> [remote-dir]
#
# Defaults to /opt/sdo-gateway. The remote host needs Docker, passwordless
# sudo for it, and an existing .env holding the secrets — this script never
# creates or overwrites .env, because that file is the one piece of state
# that is not in the repo and not reproducible.
#
# Everything else on the remote is treated as replaceable: src/, the
# Dockerfile, docker-compose.yml and the package manifests are overwritten
# from this checkout.
#
# Two things this exists to get right, both learned by getting them wrong:
#
#   1. The container's identity in the public server directory lives in
#      directory-server-id.txt. Only the directory holding players.db is a
#      volume, so a rebuild used to mint a fresh id and leak a duplicate
#      listing. The id is captured before the swap and restored after.
#
#   2. Renaming environment variables in the repo without renaming them in
#      the remote .env silently starves the new code of its config. The
#      preflight refuses to deploy when the two disagree.
#
# On any failure after the swap, the previous release is restored and the
# container brought back up on it.

set -euo pipefail

HOST="${1:?usage: deploy-remote.sh <ssh-host> [remote-dir]}"
DIR="${2:-/opt/sdo-gateway}"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"   # the server/ directory
TS="$(date +%Y%m%d-%H%M%S)"

say() { printf '\n\033[1m== %s\033[0m\n' "$*"; }
rsh() { ssh -o ConnectTimeout=15 "$HOST" "$@"; }

say "Preflight"
rsh "command -v docker >/dev/null || { echo 'docker not found'; exit 1; }
     sudo -n true 2>/dev/null   || { echo 'passwordless sudo unavailable'; exit 1; }
     [ -f '$DIR/.env' ]         || { echo 'no $DIR/.env — create it first'; exit 1; }
     echo '  docker, sudo, .env all present'"

# Compare the env var names this checkout reads against the ones the remote
# .env defines. A name the code needs and the file lacks is the failure mode
# that a rename introduces, and it produces defaults rather than an error.
NEEDED="$(grep -rhoE "SDO_[A-Z_]+" "$HERE/src" "$HERE/docker-compose.yml" | sort -u)"
HAVE="$(rsh "grep -oE '^SDO_[A-Z_]+' '$DIR/.env' | sort -u" || true)"
STALE="$(rsh "grep -coE '^SDB_' '$DIR/.env' || true")"
if [ "${STALE:-0}" -gt 0 ]; then
    echo "  ABORT: $DIR/.env still has $STALE SDB_* keys; this build reads SDO_*." >&2
    echo "  Fix with:  ssh $HOST \"sed -i 's/^SDB_/SDO_/' $DIR/.env\"" >&2
    exit 1
fi
echo "  .env defines $(echo "$HAVE" | grep -c . || true) SDO_* keys; code references $(echo "$NEEDED" | grep -c . || true)"

say "Packaging"
TAR="$(mktemp -t sdo-deploy-XXXXXX.tgz)"
trap 'rm -f "$TAR"' EXIT
tar czf "$TAR" -C "$HERE" src package.json package-lock.json docker-compose.yml Dockerfile
echo "  $(du -h "$TAR" | cut -f1)"
scp -q -o ConnectTimeout=15 "$TAR" "$HOST:/tmp/sdo-deploy.tgz"

say "Deploying"
rsh "set -euo pipefail
cd '$DIR'
BK='.backup-$TS'
mkdir -p \"\$BK\"
cp -a .env docker-compose.yml Dockerfile package.json package-lock.json \"\$BK\"/ 2>/dev/null || true
cp -a src \"\$BK\"/src
echo \"  backup: \$BK\"

# Preserve the directory identity across the rebuild (see header, note 1).
ID=\"\"
if sudo -n docker compose ps -q gateway 2>/dev/null | grep -q .; then
    ID=\$(sudo -n docker compose exec -T gateway sh -c 'cat /app/data/directory-server-id.txt 2>/dev/null || cat /app/directory-server-id.txt 2>/dev/null' 2>/dev/null | tr -d '\r\n' || true)
fi
[ -n \"\$ID\" ] && echo \"  preserving directory id \${ID:0:8}…\" || echo '  no existing directory id'

rm -rf .incoming && mkdir .incoming
tar xzf /tmp/sdo-deploy.tgz -C .incoming
rm -rf src.prev && mv src src.prev && mv .incoming/src src
cp .incoming/package.json .incoming/package-lock.json .incoming/docker-compose.yml .incoming/Dockerfile .
rm -rf .incoming /tmp/sdo-deploy.tgz

rollback() {
    echo '  !! deploy failed — rolling back'
    rm -rf src && mv src.prev src
    cp -a \"\$BK\"/docker-compose.yml \"\$BK\"/Dockerfile \"\$BK\"/package.json \"\$BK\"/package-lock.json . 2>/dev/null || true
    sudo -n docker compose up -d --build >/dev/null 2>&1 || true
    exit 1
}
trap rollback ERR

sudo -n docker compose up -d --build 2>&1 | tail -3

# Restore the identity onto the volume, then restart so the app picks it up.
if [ -n \"\$ID\" ]; then
    printf '%s' \"\$ID\" | sudo -n docker compose exec -T gateway sh -c 'cat > /app/data/directory-server-id.txt'
    sudo -n docker compose restart >/dev/null
fi

echo '  waiting for health…'
for i in \$(seq 1 30); do
    S=\$(sudo -n docker inspect --format '{{if .State.Health}}{{.State.Health.Status}}{{else}}none{{end}}' \$(sudo -n docker compose ps -q gateway) 2>/dev/null || echo none)
    [ \"\$S\" = healthy ] && { echo '  healthy'; break; }
    [ \"\$S\" = unhealthy ] && { echo '  reported unhealthy'; rollback; }
    sleep 2
done
trap - ERR
echo \"  done — previous release kept at \$BK and src.prev\"
"

say "Verifying"
rsh "cd '$DIR'; sudo -n docker compose ps --format '  {{.Name}}  {{.Status}}'; sudo -n docker compose logs --tail=6 2>&1 | tail -6"
