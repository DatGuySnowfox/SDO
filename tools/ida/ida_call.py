#!/usr/bin/env python3
"""Call the IDA MCP server over plain HTTP JSON-RPC.

The MCP transport is configured but was pointed at a dead host, so this talks to
the live one directly. Usage:

    python ida_call.py <tool> '<json args>'
"""
import sys, json, urllib.request

URL = "http://192.168.4.46:8744/mcp"


def call(tool, args, timeout=180):
    body = json.dumps({
        "jsonrpc": "2.0", "id": 1, "method": "tools/call",
        "params": {"name": tool, "arguments": args},
    }).encode()
    req = urllib.request.Request(
        URL, data=body,
        headers={"Content-Type": "application/json",
                 "Accept": "application/json, text/event-stream"})
    raw = urllib.request.urlopen(req, timeout=timeout).read().decode("utf-8", "replace")
    # The server may reply as SSE; take the last JSON object in the stream.
    start = raw.find("{")
    d = json.loads(raw[start:]) if start >= 0 else {}
    res = d.get("result", d)
    sc = res.get("structuredContent")
    if sc is not None:
        return sc
    cont = res.get("content")
    if isinstance(cont, list):
        out = []
        for c in cont:
            out.append(c.get("text", "") if isinstance(c, dict) else str(c))
        return "\n".join(out)
    return res


if __name__ == "__main__":
    tool = sys.argv[1]
    args = json.loads(sys.argv[2]) if len(sys.argv) > 2 else {}
    r = call(tool, args)
    print(r if isinstance(r, str) else json.dumps(r, indent=2)[:6000])
