# SDO server directory (Cloudflare Worker)

Free-tier server discovery: home-hosted gateways heartbeat in, players list
what's currently up. Replaces hardcoding a gateway IP in the launch scripts.

## One-time setup

```
cd directory-worker
npm install
npx wrangler login              # opens a browser, authorizes against your Cloudflare account
```

No storage provisioning step: the registry lives in a SQLite-backed Durable
Object that `wrangler deploy` creates on first deploy from the
`[exports.ServerRegistry]` block in `wrangler.toml`.

Then set the shared secret hosts must present to heartbeat in (pick any
random string - this is not a per-player secret, just a "don't let strangers
spam fake server entries" gate):

```
npx wrangler secret put DIRECTORY_KEY
```

## Deploy

```
npm run deploy
```

Prints the Worker's public URL (a `*.workers.dev` subdomain unless a custom domain route is
configured in `wrangler.toml`, as below). That URL is `SDO_DIRECTORY_URL` for host-agent and for
`scripts/join.ps1`.

## Storage

The server list is a single SQLite-backed Durable Object (`ServerRegistry`), reached by the fixed
name `v1` so every request hits the same instance.

It was originally Workers KV, which did not survive a heartbeat workload on the free plan: KV allows
**1,000 writes/day account-wide**, and a single server heartbeating on the default 60s interval is
1,440 writes/day by itself - the quota was exhausted in roughly 17 hours. Reads (100,000/day) and
storage size were never close to their limits; the problem is specific to refreshing a liveness
timestamp on a timer, which is write-shaped. Raising the heartbeat interval only defers the cliff
until the second or third community-hosted server, so the store was swapped rather than retuned.

Durable Objects bill compute (requests + duration) rather than per-write, and SQLite-backed ones are
available on the Workers Free plan. KV's per-key `expirationTtl` had no direct equivalent, so expiry
is now an explicit freshness filter plus an opportunistic delete of stale rows on each read.

### Custom domain (optional)

A bare `npm run deploy` publishes to your free `<name>.<subdomain>.workers.dev` URL, which is
enough to run a directory. To serve it from your own domain instead, add a `routes` block with a
`custom_domain = true` pattern for a zone on your own Cloudflare account; `wrangler deploy`
provisions the DNS record itself via the Workers routes API, needing no DNS access beyond what
`wrangler login` already grants. Note that adding a custom domain route disables the `workers.dev`
URL unless you also set `workers_dev = true`.

This repo intentionally ships **no** domain. The reference deployment keeps its route in an
untracked `wrangler.prod.toml` (copy `wrangler.toml`, add the routes block) and deploys with
`npm run deploy:prod`. Be aware that wrangler treats its config as the source of truth for routes,
so deploying with a config that omits your custom domain can detach it.

## API

- `POST /v1/heartbeat` - header `X-Directory-Key: <the secret>`, JSON body
  `{ serverId, name, host, port, playerCount, maxPlayers }`. Upserts the
  entry with a 90s TTL - stop heartbeating and it disappears on its own, no
  explicit despawn/cleanup needed.
- `GET /v1/servers` - public, no auth. Returns `{ ok: true, servers: [...] }`,
  sorted by name.

## Local testing

`npm run dev` runs it against Cloudflare's local Miniflare simulator
(`http://127.0.0.1:8787` by default) - same KV semantics, no need to deploy
to iterate.
