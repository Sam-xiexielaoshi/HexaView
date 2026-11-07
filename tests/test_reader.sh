#!/usr/bin/env bash

set -eu

PROGRAM="$1"

TEMP_DIR=$(mktemp -d)
trap 'rm -rf "$TEMP_DIR"' EXIT

echo "Running HexaView integration tests..."
echo ""

# Test 1: Normal text file
printf 'Hello World\n' > "$TEMP_DIR/sample.bin"

OUTPUT=$("$PROGRAM" "$TEMP_DIR/sample.bin")

echo "$OUTPUT" | grep -q "File size:  12 bytes"
echo "$OUTPUT" | grep -q "Bytes read: 12"
echo "$OUTPUT" | grep -q "48 65 6C 6C 6F"
echo "$OUTPUT" | grep -q "Hello World."

echo "[PASS] Normal text file"

# Test 2: Empty file
touch "$TEMP_DIR/empty.bin"

OUTPUT=$("$PROGRAM" "$TEMP_DIR/empty.bin")

echo "$OUTPUT" | grep -q "File size:  0 bytes"
echo "$OUTPUT" | grep -q "Bytes read: 0"

if echo "$OUTPUT" | grep -q "00000000"; then
    echo "[FAIL] Empty file should not contain hex rows"
    exit 1
fi

echo "[PASS] Empty file"

# Test 3: Binary file
printf '\x48\x65\x78\x00\x41\x42\x43\xFF' > "$TEMP_DIR/binary.bin"

OUTPUT=$("$PROGRAM" "$TEMP_DIR/binary.bin")

echo "$OUTPUT" | grep -q "48 65 78 00 41 42 43 FF"
echo "$OUTPUT" | grep -q "Hex.ABC."

echo "[PASS] Binary file"

# Test 4: Multi-row file
printf 'ABCDEFGHIJKLMNOPQRSTUVWXYZ12345678901234' > "$TEMP_DIR/multirow.bin"

OUTPUT=$("$PROGRAM" "$TEMP_DIR/multirow.bin")

echo "$OUTPUT" | grep -q "00000000"
echo "$OUTPUT" | grep -q "00000010"
echo "$OUTPUT" | grep -q "00000020"
echo "$OUTPUT" | grep -q "78901234"

echo "[PASS] Multi-row file"

# Test 5: Nonexistent file
if "$PROGRAM" "$TEMP_DIR/nonexistent.bin" > /dev/null 2>&1; then
    echo "[FAIL] Nonexistent file should produce an error"
    exit 1
fi

echo "[PASS] Nonexistent file"

# Test 6: Missing command-line argument
if "$PROGRAM" > /dev/null 2>&1; then
    echo "[FAIL] Missing argument should produce an error"
    exit 1
fi

echo "[PASS] Missing argument"

echo ""
echo "All HexaView integration tests passed!"
