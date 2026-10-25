# ─────────────────────────────────────────────────────────────────
#  Metro Train Route Management System
#  Single-file build: metro.cpp -> metro binary
# ─────────────────────────────────────────────────────────────────

# Stage 1 — Build
FROM gcc:13 AS builder

WORKDIR /app

COPY metro.cpp .

# Single source file, static-link C++ runtime so the slim runtime
# image needs no matching libstdc++ version
RUN g++ -std=c++17 -Wall -Wextra -static-libstdc++ -static-libgcc \
        -o metro metro.cpp

# ─────────────────────────────────────────────────────────────────
# Stage 2 — Runtime (slim Debian, no compiler overhead)
# ─────────────────────────────────────────────────────────────────
FROM debian:bookworm-slim AS runtime

WORKDIR /app

# Copy only the compiled binary from the build stage
COPY --from=builder /app/metro .

# The program is interactive (terminal menu), so keep stdin open
CMD ["./metro"]
