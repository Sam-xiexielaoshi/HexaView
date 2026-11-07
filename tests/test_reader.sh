
#!/usr/bin/env bash

set -eu

PROGRAM="$1"
TEMP_DIR=$(mktemp -d)

trap 'rm -rf "$TEMP_DIR"' EXIT

echo "Running HexaView tests..."

# Test 1: Normal file
printf 'Hello World' > "$TEMP_DIR/sample.bin"

OUTPUT=$("$PROGRAM" "$TEMP_DIR/sample.bin")

printf '%s\n' "$OUTPUT" | grep -q "File size:  11 bytes"
printf '%s\n' "$OUTPUT" | grep -q "Bytes read: 11"
printf '%s\n' "$OUTPUT" | grep -q "Chunks:     1"

echo "[PASS] Normal file"

# Test 2: Empty file
touch "$TEMP_DIR/empty.bin"

OUTPUT=$("$PROGRAM" "$TEMP_DIR/empty.bin")

printf '%s\n' "$OUTPUT" | grep -q "File size:  0 bytes"
printf '%s\n' "$OUTPUT" | grep -q "Bytes read: 0"
printf '%s\n' "$OUTPUT" | grep -q "Chunks:     0"

echo "[PASS] Empty file"

# Test 3: Invalid file path
if "$PROGRAM" "$TEMP_DIR/nonexistent.bin" > /dev/null 2>&1; then
    echo "[FAIL] Invalid file path"
    exit 1
fi

echo "[PASS] Invalid file path"

# Test 4: Missing argument
if "$PROGRAM" > /dev/null 2>&1; then
    echo "[FAIL] Missing argument"
    exit 1
fi

echo "[PASS] Missing argument"

echo ""
echo "All tests passed!"
