FROM node:22-slim

# better-sqlite3 has a native binding — no prebuilt binary is available for
# this base image, so it compiles from source via node-gyp, which needs
# these. Not needed at runtime, but keeping a single-stage build simple is
# worth the extra ~150MB image size over a multi-stage split for a small
# personal deployment like this.
RUN apt-get update && apt-get install -y --no-install-recommends \
    python3 make g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY package.json package-lock.json ./
RUN npm install --omit=dev
COPY src ./src

EXPOSE 31000 31001
CMD ["node", "src/index.js"]
