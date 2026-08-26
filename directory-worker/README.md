# SDO server directory (Cloudflare Worker)

Free-tier server discovery: home-hosted gateways heartbeat in, players list
what's currently up. Replaces hardcoding a gateway IP in the launch scripts.

## One-time setup

```
cd directory-worker
npm install
npx wrangler login              # opens a browser, authorizes against your Cloudflare account
npx wrangler kv namespace create SERVERS
```

The last command prints an `id = "..."` — paste it into `wrangler.toml`'s
`kv_namespaces` entry (replacing `REPLACE_WITH_KV_NAMESPACE_ID`).

Then set the shared secret hosts must present to heartbeat in (pick any
random string — this is not a per-player secret, just a "don't let strangers
spam fake server entries" gate):

```
npx wrangler secret put DIRECTORY_KEY
```

## Deploy

```
npm run deploy
```

Prints the Worker's public URL (a `*.workers.dev` subdomain unless a custom domain route is
configured in `wrangler.toml`, as below). That URL is `SDB_DIRECTORY_URL` for host-agent and for
`scripts/join.ps1`.

**Deployed 2026-08-21**: live at `https://sdo.ristl.org` (custom domain, `ristl.org`'s zone is on
the same Cloudflare account — added via the `routes` block in `wrangler.toml`, no separate DNS
access needed beyond the existing `wrangler login` token's `workers_routes:write` scope). Adding a
custom domain route disables the `workers.dev` URL by default unless `workers_dev = true` is also
set.

## API

- `POST /v1/heartbeat` — header `X-Directory-Key: <the secret>`, JSON body
  `{ serverId, name, host, port, playerCount, maxPlayers }`. Upserts the
  entry with a 90s TTL — stop heartbeating and it disappears on its own, no
  explicit despawn/cleanup needed.
- `GET /v1/servers` — public, no auth. Returns `{ ok: true, servers: [...] }`,
  sorted by name.

## Local testing

`npm run dev` runs it against Cloudflare's local Miniflare simulator
(`http://127.0.0.1:8787` by default) — same KV semantics, no need to deploy
to iterate.
