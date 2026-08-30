# Build stage - compile WebAssembly
FROM emscripten/emsdk:latest AS wasm-builder

WORKDIR /app

# Copy source files
COPY main.cpp .
COPY CMakeLists.txt .

# Create build directory and compile
RUN mkdir -p build && \
    cd build && \
    emcmake cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . && \
    cd .. && \
    mkdir -p public && \
    cp build/game.js public/ && \
    cp build/game.wasm public/ || true

# Runtime stage
FROM node:18-alpine

WORKDIR /app

# Copy package files
COPY package*.json ./

# Install dependencies
RUN npm ci --only=production

# Copy compiled WASM from builder
COPY --from=wasm-builder /app/public ./public

# Copy application code
COPY server.js .
COPY client.js public/
COPY shell.html public/
COPY public/index.html public/

# Expose port
EXPOSE 3000

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD node -e "require('http').get('http://localhost:3000/api/status', (r) => {if (r.statusCode !== 200) throw new Error(r.statusCode)})"

# Start application
CMD ["npm", "start"]
