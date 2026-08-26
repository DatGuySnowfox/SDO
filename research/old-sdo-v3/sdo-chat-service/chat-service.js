'use strict';

const http = require('http');
const fs = require('fs');
const path = require('path');
const crypto = require('crypto');
const { URL } = require('url');

const HOST = process.env.HOST || '127.0.0.1';
const PORT = Number.parseInt(process.env.PORT || '27884', 10);
const STATE_DIR =
  process.env.STATE_DIR || '/var/lib/sdo-chat-service';
const STATE_FILE = path.join(STATE_DIR, 'chat-state.json');

const VERSION = '1.0.0-global-chat';
const MAX_REQUEST_BYTES = 16 * 1024;
const MAX_MESSAGES = 200;
const MAX_RETURNED_MESSAGES = 50;
const MAX_TEXT_LENGTH = 180;
const MAX_DISPLAY_NAME_LENGTH = 32;
const MESSAGE_COOLDOWN_MS = 1200;

function emptyState() {
  return {
    version: 1,
    nextMessageId: 1,
    profiles: {},
    messages: []
  };
}

function ensureStateDir() {
  fs.mkdirSync(STATE_DIR, {
    recursive: true,
    mode: 0o700
  });
}

function loadState() {
  ensureStateDir();

  if (!fs.existsSync(STATE_FILE)) {
    return emptyState();
  }

  try {
    const parsed = JSON.parse(
      fs.readFileSync(STATE_FILE, 'utf8')
    );

    if (
      !parsed ||
      typeof parsed !== 'object' ||
      !parsed.profiles ||
      !Array.isArray(parsed.messages)
    ) {
      return emptyState();
    }

    parsed.nextMessageId = Number.isSafeInteger(
      parsed.nextMessageId
    )
      ? parsed.nextMessageId
      : 1;

    return parsed;
  } catch (error) {
    const broken =
      STATE_FILE + '.broken-' + Date.now().toString();

    try {
      fs.renameSync(STATE_FILE, broken);
    } catch (_) {
      // Leave the broken file in place if it cannot be moved.
    }

    return emptyState();
  }
}

let state = loadState();

function saveState() {
  ensureStateDir();

  const temporary =
    STATE_FILE + '.tmp-' + process.pid.toString();

  fs.writeFileSync(
    temporary,
    JSON.stringify(state, null, 2),
    {
      encoding: 'utf8',
      mode: 0o600
    }
  );

  fs.renameSync(temporary, STATE_FILE);
}

function sha256(value) {
  return crypto
    .createHash('sha256')
    .update(value, 'utf8')
    .digest('hex');
}

function constantTimeEqual(left, right) {
  const leftBuffer = Buffer.from(String(left));
  const rightBuffer = Buffer.from(String(right));

  if (leftBuffer.length !== rightBuffer.length) {
    return false;
  }

  return crypto.timingSafeEqual(
    leftBuffer,
    rightBuffer
  );
}

function cleanSingleLine(value, maximumLength) {
  let text = String(value || '')
    .replace(/[\u0000-\u001f\u007f]/g, ' ')
    .replace(/\s+/g, ' ')
    .trim();

  if (text.length > maximumLength) {
    text = text.slice(0, maximumLength);
  }

  return text;
}

function cleanDisplayName(value) {
  return (
    cleanSingleLine(
      value,
      MAX_DISPLAY_NAME_LENGTH
    ) || 'Survivor'
  );
}

function cleanMessage(value) {
  return cleanSingleLine(
    value,
    MAX_TEXT_LENGTH
  );
}

function validPlayerId(value) {
  return /^p_[A-Za-z0-9_-]{16,96}$/.test(
    String(value || '')
  );
}

function validProfileToken(value) {
  return /^[A-Fa-f0-9]{32,256}$/.test(
    String(value || '')
  );
}

function sendJson(response, statusCode, value) {
  const body = Buffer.from(
    JSON.stringify(value),
    'utf8'
  );

  response.writeHead(statusCode, {
    'Content-Type': 'application/json; charset=utf-8',
    'Content-Length': body.length,
    'Cache-Control': 'no-store',
    'X-Content-Type-Options': 'nosniff'
  });

  response.end(body);
}

function readJson(request) {
  return new Promise((resolve, reject) => {
    const chunks = [];
    let total = 0;

    request.on('data', (chunk) => {
      total += chunk.length;

      if (total > MAX_REQUEST_BYTES) {
        reject(
          Object.assign(
            new Error('Request body is too large.'),
            { statusCode: 413 }
          )
        );
        request.destroy();
        return;
      }

      chunks.push(chunk);
    });

    request.on('end', () => {
      try {
        const raw = Buffer.concat(chunks)
          .toString('utf8');

        resolve(
          raw.length > 0
            ? JSON.parse(raw)
            : {}
        );
      } catch (_) {
        reject(
          Object.assign(
            new Error('Invalid JSON body.'),
            { statusCode: 400 }
          )
        );
      }
    });

    request.on('error', reject);
  });
}

function bearerToken(request) {
  const authorization = String(
    request.headers.authorization || ''
  );

  const match = authorization.match(
    /^Bearer\s+(.+)$/i
  );

  return match ? match[1].trim() : '';
}

function authenticate(
  request,
  response,
  playerId
) {
  if (!validPlayerId(playerId)) {
    sendJson(response, 400, {
      ok: false,
      error: 'invalid-player-id'
    });
    return null;
  }

  const profile = state.profiles[playerId];

  if (!profile) {
    sendJson(response, 401, {
      ok: false,
      error: 'profile-not-registered'
    });
    return null;
  }

  const supplied = bearerToken(request);

  if (
    !validProfileToken(supplied) ||
    !constantTimeEqual(
      profile.tokenHash,
      sha256(supplied)
    )
  ) {
    sendJson(response, 401, {
      ok: false,
      error: 'invalid-token'
    });
    return null;
  }

  return profile;
}

function registerProfile(
  response,
  body
) {
  const playerId = String(
    body.playerId || ''
  );
  const profileToken = String(
    body.profileToken || ''
  );
  const displayName = cleanDisplayName(
    body.displayName
  );

  if (
    !validPlayerId(playerId) ||
    !validProfileToken(profileToken)
  ) {
    sendJson(response, 400, {
      ok: false,
      error: 'invalid-registration'
    });
    return;
  }

  const tokenHash = sha256(profileToken);
  const existing = state.profiles[playerId];

  if (
    existing &&
    !constantTimeEqual(
      existing.tokenHash,
      tokenHash
    )
  ) {
    sendJson(response, 403, {
      ok: false,
      error: 'player-id-already-registered'
    });
    return;
  }

  const now = new Date().toISOString();

  state.profiles[playerId] = {
    tokenHash,
    displayName,
    createdAt:
      existing?.createdAt || now,
    updatedAt: now,
    lastMessageAt:
      Number(existing?.lastMessageAt || 0)
  };

  saveState();

  sendJson(response, 200, {
    ok: true,
    playerId,
    displayName,
    service: 'sdo-global-chat',
    version: VERSION
  });
}

function listMessages(
  request,
  response,
  requestUrl
) {
  const playerId =
    requestUrl.searchParams.get('playerId') || '';

  const profile = authenticate(
    request,
    response,
    playerId
  );

  if (!profile) {
    return;
  }

  const parsedAfter = Number.parseInt(
    requestUrl.searchParams.get('afterId') || '0',
    10
  );

  const afterId = Number.isSafeInteger(
    parsedAfter
  )
    ? Math.max(0, parsedAfter)
    : 0;

  let messages = state.messages.filter(
    (message) => message.id > afterId
  );

  if (
    afterId === 0 &&
    messages.length > MAX_RETURNED_MESSAGES
  ) {
    messages = messages.slice(
      -MAX_RETURNED_MESSAGES
    );
  } else if (
    messages.length > MAX_RETURNED_MESSAGES
  ) {
    messages = messages.slice(
      0,
      MAX_RETURNED_MESSAGES
    );
  }

  sendJson(response, 200, {
    ok: true,
    messages: messages.map(
      ({
        id,
        createdAt,
        playerId: senderPlayerId,
        displayName,
        text
      }) => ({
        id,
        createdAt,
        playerId: senderPlayerId,
        displayName,
        text
      })
    )
  });
}

function postMessage(
  request,
  response,
  body
) {
  const playerId = String(
    body.playerId || ''
  );

  const profile = authenticate(
    request,
    response,
    playerId
  );

  if (!profile) {
    return;
  }

  const text = cleanMessage(body.text);
  const clientMessageId = cleanSingleLine(
    body.clientMessageId,
    96
  );

  if (!text) {
    sendJson(response, 400, {
      ok: false,
      error: 'empty-message'
    });
    return;
  }

  if (!clientMessageId) {
    sendJson(response, 400, {
      ok: false,
      error: 'missing-client-message-id'
    });
    return;
  }

  const duplicate = state.messages.find(
    (message) =>
      message.playerId === playerId &&
      message.clientMessageId ===
        clientMessageId
  );

  if (duplicate) {
    sendJson(response, 200, {
      ok: true,
      duplicate: true,
      message: duplicate
    });
    return;
  }

  const nowMilliseconds = Date.now();
  const previous = Number(
    profile.lastMessageAt || 0
  );

  if (
    nowMilliseconds - previous <
    MESSAGE_COOLDOWN_MS
  ) {
    sendJson(response, 429, {
      ok: false,
      error: 'message-rate-limited',
      retryAfterMs:
        MESSAGE_COOLDOWN_MS -
        (nowMilliseconds - previous)
    });
    return;
  }

  const message = {
    id: state.nextMessageId,
    createdAt: new Date(
      nowMilliseconds
    ).toISOString(),
    playerId,
    displayName:
      cleanDisplayName(profile.displayName),
    text,
    clientMessageId
  };

  state.nextMessageId += 1;
  profile.lastMessageAt =
    nowMilliseconds;
  profile.updatedAt =
    message.createdAt;

  state.messages.push(message);

  if (
    state.messages.length >
    MAX_MESSAGES
  ) {
    state.messages = state.messages.slice(
      -MAX_MESSAGES
    );
  }

  saveState();

  sendJson(response, 201, {
    ok: true,
    duplicate: false,
    message
  });
}

const server = http.createServer(
  async (request, response) => {
    try {
      const requestUrl = new URL(
        request.url,
        `http://${request.headers.host || 'localhost'}`
      );

      if (
        request.method === 'GET' &&
        requestUrl.pathname === '/health'
      ) {
        sendJson(response, 200, {
          ok: true,
          service: 'sdo-global-chat',
          version: VERSION,
          registeredPlayers:
            Object.keys(
              state.profiles
            ).length,
          retainedMessages:
            state.messages.length,
          maxMessageLength:
            MAX_TEXT_LENGTH,
          cooldownMs:
            MESSAGE_COOLDOWN_MS
        });
        return;
      }

      if (
        request.method === 'POST' &&
        requestUrl.pathname ===
          '/v1/register'
      ) {
        registerProfile(
          response,
          await readJson(request)
        );
        return;
      }

      if (
        request.method === 'GET' &&
        requestUrl.pathname ===
          '/v1/messages'
      ) {
        listMessages(
          request,
          response,
          requestUrl
        );
        return;
      }

      if (
        request.method === 'POST' &&
        requestUrl.pathname ===
          '/v1/messages'
      ) {
        postMessage(
          request,
          response,
          await readJson(request)
        );
        return;
      }

      sendJson(response, 404, {
        ok: false,
        error: 'not-found'
      });
    } catch (error) {
      const statusCode =
        Number(error.statusCode) || 500;

      if (statusCode >= 500) {
        console.error(
          new Date().toISOString(),
          error
        );
      }

      if (!response.headersSent) {
        sendJson(response, statusCode, {
          ok: false,
          error:
            statusCode >= 500
              ? 'internal-error'
              : error.message
        });
      } else {
        response.end();
      }
    }
  }
);

server.requestTimeout = 15000;
server.headersTimeout = 16000;
server.keepAliveTimeout = 5000;

server.listen(PORT, HOST, () => {
  console.log(
    JSON.stringify({
      ok: true,
      service: 'sdo-global-chat',
      version: VERSION,
      host: HOST,
      port: PORT,
      stateFile: STATE_FILE
    })
  );
});
