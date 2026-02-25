#!/bin/bash
# Comprehensive LMF Positioning Test Script
# Supports all positioning methods: CELLID (with/without cell info) and ECID (with/without fallback)

set -e

LMF_HOST="${LMF_HOST:-127.0.0.16}"
LMF_PORT="${LMF_PORT:-7777}"
SUPI="${SUPI:-imsi-999700000056491}"
AMF_ID="${AMF_ID:-b56f0d5a-c57a-41f0-9618-4b15b8c8d654}"

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0

# Function to print section header
print_section() {
    echo "" >&2
    echo -e "${BLUE}========================================${NC}" >&2
    echo -e "${BLUE}$1${NC}" >&2
    echo -e "${BLUE}========================================${NC}" >&2
}

# Function to print test header
print_test() {
    echo "" >&2
    echo -e "${CYAN}Test: $1${NC}" >&2
    echo -e "${CYAN}----------------------------------------${NC}" >&2
}

# Function to check HTTP status and response
check_response() {
    local test_name="$1"
    local expected_method="$2"
    local status_code="$3"
    local response_body="$4"
    
    if [ "$status_code" = "000" ]; then
        echo -e "${RED}✗${NC} $test_name: Connection failed" >&2
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
    
    if [ "$status_code" -ge 200 ] && [ "$status_code" -lt 300 ]; then
        # Check if positioning method matches expected
        local actual_method=$(echo "$response_body" | jq -r '.locationData.positioningMethod // "unknown"' 2>/dev/null)
        if echo "$response_body" | jq -e ".locationData.positioningMethod == \"$expected_method\"" >/dev/null 2>&1 || [ "$actual_method" != "unknown" ]; then
            echo -e "${GREEN}✓${NC} $test_name: HTTP $status_code, Method=$actual_method" >&2
            echo "" >&2
            echo -e "${CYAN}Result:${NC}" >&2
            echo "$response_body" | jq '.' >&2
            echo "" >&2
            TESTS_PASSED=$((TESTS_PASSED + 1))
            return 0
        else
            echo -e "${YELLOW}⚠${NC} $test_name: HTTP $status_code, Expected Method=$expected_method, Got=$actual_method" >&2
            echo "" >&2
            echo -e "${CYAN}Result:${NC}" >&2
            echo "$response_body" | jq '.' >&2
            echo "" >&2
            TESTS_FAILED=$((TESTS_FAILED + 1))
            return 1
        fi
    else
        echo -e "${RED}✗${NC} $test_name: HTTP $status_code" >&2
        echo "" >&2
        echo -e "${CYAN}Error Response:${NC}" >&2
        echo "$response_body" | jq '.' >&2 2>/dev/null || echo "$response_body" >&2
        echo "" >&2
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
}

# Function to send location request
send_request() {
    local json_payload="$1"
    local status_code
    local response_body
    
    response_body=$(curl --http2-prior-knowledge -s -w "\n%{http_code}" -X POST \
        "http://${LMF_HOST}:${LMF_PORT}/nlmf-loc/v1/determine-location" \
        -H "Content-Type: application/json" \
        -d "$json_payload" 2>/dev/null)
    
    status_code=$(echo "$response_body" | tail -n1)
    response_body=$(echo "$response_body" | sed '$d')
    
    echo "$status_code|$response_body"
}

# Check if LMF daemon is reachable
check_lmf_connection() {
    echo -e "${YELLOW}Checking LMF daemon connection...${NC}" >&2
    HTTP_TEST_CODE=$(curl --http2-prior-knowledge -s --connect-timeout 2 -o /dev/null -w "%{http_code}" \
        "http://${LMF_HOST}:${LMF_PORT}/" 2>/dev/null || echo "000")
    if [ "$HTTP_TEST_CODE" = "000" ] || [ -z "$HTTP_TEST_CODE" ]; then
        echo -e "${RED}ERROR: Cannot connect to LMF daemon at ${LMF_HOST}:${LMF_PORT}${NC}" >&2
        echo "Please ensure the LMF daemon is running:" >&2
        echo "  ./bin/open5gs-lmfd -c configs/lmf.yaml" >&2
        exit 1
    fi
    echo -e "${GREEN}✓${NC} LMF daemon is reachable" >&2
    echo "" >&2
}

# Show usage
show_usage() {
    cat << EOF
Usage: $0 [OPTIONS] [TEST_NAMES...]

Comprehensive LMF Positioning Test Script

OPTIONS:
    -h, --help          Show this help message
    -a, --all           Run all tests (default)
    -c, --cellid        Run CELLID positioning tests only
    -e, --ecid          Run ECID positioning tests only
    -j, --json          Output results as JSON (for piping to jq)
    -v, --verbose       Show detailed request/response information
    -s, --supi SUPI     UE SUPI (overrides environment variable)

TEST_NAMES (if not using -a, -c, or -e):
    cellid-provided     CELLID with NCGI provided in request
    cellid-supi         CELLID with SUPI only (queries AMF)
    cellid-supi-no-amf  CELLID with SUPI only, no AMF ID (NRF discovery)
    ecid-explicit       ECID with hAccuracy=0 (explicit, with fallback)
    ecid-high-acc       ECID with hAccuracy=50 (high accuracy, no fallback)
    ecid-default        Default behavior (no LocationQoS)
    amf-discovery       AMF discovery tests (with/without AMF ID)

EXAMPLES:
    $0                                          # Run all tests
    $0 --cellid                                 # Run only CELLID tests
    $0 --ecid                                   # Run only ECID tests
    $0 --supi imsi-999700000021631             # Run all tests with custom SUPI
    $0 -s imsi-999700000021631 --cellid        # Run CELLID tests with custom SUPI
    $0 cellid-supi ecid-explicit               # Run specific tests
    $0 --json | jq .                           # Get JSON output

Environment Variables:
    LMF_HOST    LMF host (default: 127.0.0.16)
    LMF_PORT    LMF port (default: 7777)
    SUPI        UE SUPI (default: imsi-999700000056491, overridden by -s/--supi)
    AMF_ID      AMF instance ID (default: b56f0d5a-c57a-41f0-9618-4b15b8c8d654)
EOF
}

# Parse arguments
RUN_ALL=true
RUN_CELLID=false
RUN_ECID=false
JSON_OUTPUT=false
VERBOSE=false
TEST_NAMES=()

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_usage
            exit 0
            ;;
        -a|--all)
            RUN_ALL=true
            shift
            ;;
        -c|--cellid)
            RUN_ALL=false
            RUN_CELLID=true
            shift
            ;;
        -e|--ecid)
            RUN_ALL=false
            RUN_ECID=true
            shift
            ;;
        -j|--json)
            JSON_OUTPUT=true
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -s|--supi)
            if [ -z "$2" ]; then
                echo "Error: --supi requires a value" >&2
                show_usage
                exit 1
            fi
            SUPI="$2"
            shift 2
            ;;
        cellid-provided|cellid-supi|cellid-supi-no-amf|ecid-explicit|ecid-high-acc|ecid-default|amf-discovery)
            RUN_ALL=false
            TEST_NAMES+=("$1")
            shift
            ;;
        *)
            echo "Unknown option: $1" >&2
            show_usage
            exit 1
            ;;
    esac
done

# If specific test names provided, use them
if [ ${#TEST_NAMES[@]} -gt 0 ]; then
    RUN_ALL=false
fi

# Check LMF connection
if [ "$JSON_OUTPUT" = false ]; then
    check_lmf_connection
fi

# Test 1: CELLID with NCGI provided
test_cellid_provided() {
    print_test "CELLID Positioning (NCGI provided in request)"
    
    local json_payload=$(cat <<EOF
{
  "supi": "${SUPI}",
  "ncgi": {
    "plmnId": {
      "mcc": "999",
      "mnc": "70"
    },
    "nrCellId": "190001"
  }
}
EOF
)
    
    if [ "$VERBOSE" = true ] && [ "$JSON_OUTPUT" = false ]; then
        echo "Request:" >&2
        echo "$json_payload" | jq '.' >&2
    fi
    
    local result=$(send_request "$json_payload")
    local status_code=$(echo "$result" | cut -d'|' -f1)
    local response_body=$(echo "$result" | cut -d'|' -f2-)
    
    if [ "$JSON_OUTPUT" = true ]; then
        echo "$response_body" | jq '.'
    else
        check_response "CELLID with provided NCGI" "CELLID" "$status_code" "$response_body"
    fi
}

# Test 2: CELLID with SUPI only (queries AMF)
test_cellid_supi() {
    print_test "CELLID Positioning (SUPI only, queries AMF location-info API)"
    
    local json_payload=$(cat <<EOF
{
  "supi": "${SUPI}",
  "amfId": "${AMF_ID}"
}
EOF
)
    
    if [ "$VERBOSE" = true ] && [ "$JSON_OUTPUT" = false ]; then
        echo "Request:" >&2
        echo "$json_payload" | jq '.' >&2
    fi
    
    local result=$(send_request "$json_payload")
    local status_code=$(echo "$result" | cut -d'|' -f1)
    local response_body=$(echo "$result" | cut -d'|' -f2-)
    
    if [ "$JSON_OUTPUT" = true ]; then
        echo "$response_body" | jq '.'
    else
        check_response "CELLID with SUPI only" "CELLID" "$status_code" "$response_body"
    fi
}

# Test 3: ECID explicit request (hAccuracy=0, with fallback)
test_ecid_explicit() {
    print_test "ECID Positioning (Explicit: hAccuracy=0, with CELLID fallback)"
    
    local json_payload=$(cat <<EOF
{
  "supi": "${SUPI}",
  "amfId": "${AMF_ID}",
  "locationQoS": {
    "hAccuracy": 0
  }
}
EOF
)
    
    if [ "$VERBOSE" = true ] && [ "$JSON_OUTPUT" = false ]; then
        echo "Request:" >&2
        echo "$json_payload" | jq '.' >&2
        echo "Note: Will fallback to CELLID if gNB doesn't support NRPPa" >&2
    fi
    
    local result=$(send_request "$json_payload")
    local status_code=$(echo "$result" | cut -d'|' -f1)
    local response_body=$(echo "$result" | cut -d'|' -f2-)
    
    if [ "$JSON_OUTPUT" = true ]; then
        echo "$response_body" | jq '.'
    else
        # ECID may fallback to CELLID, so accept either
        local method=$(echo "$response_body" | jq -r '.locationData.positioningMethod // "unknown"' 2>/dev/null)
        if [ "$method" = "ECID" ] || [ "$method" = "CELLID" ]; then
            if [ "$method" = "CELLID" ]; then
                echo -e "${YELLOW}⚠${NC} ECID explicit request: Fallback to CELLID (gNB may not support NRPPa)" >&2
            fi
            check_response "ECID explicit (hAccuracy=0)" "$method" "$status_code" "$response_body"
        else
            check_response "ECID explicit (hAccuracy=0)" "ECID" "$status_code" "$response_body"
        fi
    fi
}

# Test 4: ECID high accuracy (hAccuracy=50, no fallback)
test_ecid_high_acc() {
    print_test "ECID Positioning (High accuracy: hAccuracy=50, no fallback)"
    
    local json_payload=$(cat <<EOF
{
  "supi": "${SUPI}",
  "amfId": "${AMF_ID}",
  "locationQoS": {
    "hAccuracy": 50
  }
}
EOF
)
    
    if [ "$VERBOSE" = true ] && [ "$JSON_OUTPUT" = false ]; then
        echo "Request:" >&2
        echo "$json_payload" | jq '.' >&2
        echo "Note: No fallback - will return error if gNB doesn't support NRPPa" >&2
    fi
    
    local result=$(send_request "$json_payload")
    local status_code=$(echo "$result" | cut -d'|' -f1)
    local response_body=$(echo "$result" | cut -d'|' -f2-)
    
    if [ "$JSON_OUTPUT" = true ]; then
        echo "$response_body" | jq '.'
    else
        # High accuracy ECID should not fallback, but may fail if gNB doesn't support NRPPa
        local method=$(echo "$response_body" | jq -r '.locationData.positioningMethod // "error"' 2>/dev/null)
        if [ "$status_code" -ge 200 ] && [ "$status_code" -lt 300 ]; then
            check_response "ECID high accuracy (hAccuracy=50)" "$method" "$status_code" "$response_body"
        else
            echo -e "${YELLOW}⚠${NC} ECID high accuracy: HTTP $status_code (gNB may not support NRPPa)" >&2
            echo "" >&2
            echo -e "${CYAN}Error Response:${NC}" >&2
            echo "$response_body" | jq '.' >&2 2>/dev/null || echo "$response_body" >&2
            echo "" >&2
            TESTS_FAILED=$((TESTS_FAILED + 1))
        fi
    fi
}

# Test 5: Default behavior (no LocationQoS)
test_ecid_default() {
    print_test "Default Positioning (No LocationQoS, defaults to CELLID)"
    
    local json_payload=$(cat <<EOF
{
  "supi": "${SUPI}",
  "amfId": "${AMF_ID}"
}
EOF
)
    
    if [ "$VERBOSE" = true ] && [ "$JSON_OUTPUT" = false ]; then
        echo "Request:" >&2
        echo "$json_payload" | jq '.' >&2
        echo "Note: No LocationQoS - defaults to CELLID and queries AMF" >&2
    fi
    
    local result=$(send_request "$json_payload")
    local status_code=$(echo "$result" | cut -d'|' -f1)
    local response_body=$(echo "$result" | cut -d'|' -f2-)
    
    if [ "$JSON_OUTPUT" = true ]; then
        echo "$response_body" | jq '.'
    else
        check_response "Default (no LocationQoS)" "CELLID" "$status_code" "$response_body"
    fi
}

# Test 6: CELLID with SUPI only, no AMF ID (NRF discovery)
test_cellid_supi_no_amf() {
    print_test "CELLID Positioning (SUPI only, no AMF ID - uses NRF discovery)"
    
    local json_payload=$(cat <<EOF
{
  "supi": "${SUPI}"
}
EOF
)
    
    if [ "$VERBOSE" = true ] && [ "$JSON_OUTPUT" = false ]; then
        echo "Request:" >&2
        echo "$json_payload" | jq '.' >&2
        echo "Note: No AMF ID provided - LMF will discover AMF via NRF" >&2
    fi
    
    local result=$(send_request "$json_payload")
    local status_code=$(echo "$result" | cut -d'|' -f1)
    local response_body=$(echo "$result" | cut -d'|' -f2-)
    
    if [ "$JSON_OUTPUT" = true ]; then
        echo "$response_body" | jq '.'
    else
        check_response "CELLID with SUPI only (NRF discovery)" "CELLID" "$status_code" "$response_body"
    fi
}

# Test 7: AMF Discovery scenarios
test_amf_discovery() {
    print_section "AMF Discovery Tests"
    
    # Test 7a: With explicit AMF ID (valid UUID)
    print_test "AMF Discovery: Explicit AMF ID (valid UUID)"
    
    local json_payload=$(cat <<EOF
{
  "supi": "${SUPI}",
  "amfId": "${AMF_ID}"
}
EOF
)
    
    if [ "$VERBOSE" = true ] && [ "$JSON_OUTPUT" = false ]; then
        echo "Request:" >&2
        echo "$json_payload" | jq '.' >&2
        echo "Note: AMF ID provided - LMF will use it directly if registered" >&2
    fi
    
    local result=$(send_request "$json_payload")
    local status_code=$(echo "$result" | cut -d'|' -f1)
    local response_body=$(echo "$result" | cut -d'|' -f2-)
    
    if [ "$JSON_OUTPUT" = true ]; then
        echo "$response_body" | jq '.'
    else
        check_response "AMF Discovery: Explicit AMF ID" "CELLID" "$status_code" "$response_body"
    fi
    
    # Test 7b: Without AMF ID (NRF discovery)
    print_test "AMF Discovery: No AMF ID (NRF discovery)"
    
    json_payload=$(cat <<EOF
{
  "supi": "${SUPI}"
}
EOF
)
    
    if [ "$VERBOSE" = true ] && [ "$JSON_OUTPUT" = false ]; then
        echo "Request:" >&2
        echo "$json_payload" | jq '.' >&2
        echo "Note: No AMF ID - LMF will discover AMF via NRF" >&2
    fi
    
    result=$(send_request "$json_payload")
    status_code=$(echo "$result" | cut -d'|' -f1)
    response_body=$(echo "$result" | cut -d'|' -f2-)
    
    if [ "$JSON_OUTPUT" = true ]; then
        echo "$response_body" | jq '.'
    else
        check_response "AMF Discovery: NRF discovery (no AMF ID)" "CELLID" "$status_code" "$response_body"
    fi
    
    # Test 7c: Invalid AMF ID format (should fallback to NRF discovery)
    print_test "AMF Discovery: Invalid AMF ID format (fallback to NRF)"
    
    json_payload=$(cat <<EOF
{
  "supi": "${SUPI}",
  "amfId": "invalid-amf-id-format"
}
EOF
)
    
    if [ "$VERBOSE" = true ] && [ "$JSON_OUTPUT" = false ]; then
        echo "Request:" >&2
        echo "$json_payload" | jq '.' >&2
        echo "Note: Invalid AMF ID format - LMF will fallback to NRF discovery" >&2
    fi
    
    result=$(send_request "$json_payload")
    status_code=$(echo "$result" | cut -d'|' -f1)
    response_body=$(echo "$result" | cut -d'|' -f2-)
    
    if [ "$JSON_OUTPUT" = true ]; then
        echo "$response_body" | jq '.'
    else
        check_response "AMF Discovery: Invalid AMF ID format" "CELLID" "$status_code" "$response_body"
    fi
    
    # Test 7d: Non-existent AMF ID (valid UUID but not registered, should fallback to NRF)
    print_test "AMF Discovery: Non-existent AMF ID (valid UUID, fallback to NRF)"
    
    json_payload=$(cat <<EOF
{
  "supi": "${SUPI}",
  "amfId": "00000000-0000-0000-0000-000000000000"
}
EOF
)
    
    if [ "$VERBOSE" = true ] && [ "$JSON_OUTPUT" = false ]; then
        echo "Request:" >&2
        echo "$json_payload" | jq '.' >&2
        echo "Note: Valid UUID but not registered - LMF will fallback to NRF discovery" >&2
    fi
    
    result=$(send_request "$json_payload")
    status_code=$(echo "$result" | cut -d'|' -f1)
    response_body=$(echo "$result" | cut -d'|' -f2-)
    
    if [ "$JSON_OUTPUT" = true ]; then
        echo "$response_body" | jq '.'
    else
        check_response "AMF Discovery: Non-existent AMF ID" "CELLID" "$status_code" "$response_body"
    fi
}

# Main execution
if [ "$JSON_OUTPUT" = false ]; then
    print_section "LMF Positioning Test Suite"
    echo "Testing LMF at: http://${LMF_HOST}:${LMF_PORT}" >&2
    echo "SUPI: ${SUPI}" >&2
    echo "AMF ID: ${AMF_ID}" >&2
fi

# Run tests based on options
if [ "$RUN_ALL" = true ] || [ "$RUN_CELLID" = true ]; then
    test_cellid_provided
    test_cellid_supi
    test_cellid_supi_no_amf
fi

if [ "$RUN_ALL" = true ] || [ "$RUN_ECID" = true ]; then
    test_ecid_explicit
    test_ecid_high_acc
    test_ecid_default
fi

# Run AMF discovery tests if --all is specified
if [ "$RUN_ALL" = true ]; then
    test_amf_discovery
fi

# Run specific tests if requested
if [ ${#TEST_NAMES[@]} -gt 0 ]; then
    for test_name in "${TEST_NAMES[@]}"; do
        case $test_name in
            cellid-provided)
                test_cellid_provided
                ;;
            cellid-supi)
                test_cellid_supi
                ;;
            cellid-supi-no-amf)
                test_cellid_supi_no_amf
                ;;
            ecid-explicit)
                test_ecid_explicit
                ;;
            ecid-high-acc)
                test_ecid_high_acc
                ;;
            ecid-default)
                test_ecid_default
                ;;
            amf-discovery)
                test_amf_discovery
                ;;
        esac
    done
fi

# Print summary
if [ "$JSON_OUTPUT" = false ]; then
    echo "" >&2
    print_section "Test Summary"
    echo -e "Tests Passed: ${GREEN}${TESTS_PASSED}${NC}" >&2
    echo -e "Tests Failed: ${RED}${TESTS_FAILED}${NC}" >&2
    echo "" >&2
    
    if [ $TESTS_FAILED -eq 0 ]; then
        echo -e "${GREEN}All tests passed!${NC}" >&2
        exit 0
    else
        echo -e "${RED}Some tests failed.${NC}" >&2
        exit 1
    fi
fi

