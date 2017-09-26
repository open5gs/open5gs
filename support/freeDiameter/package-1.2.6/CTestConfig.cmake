# Configuration for sumitting the test result.
# Check http://www.freediameter.net/CDash for detail.

set(CTEST_PROJECT_NAME "freeDiameter")
set(CTEST_NIGHTLY_START_TIME "00:00:00 JST")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "www.freediameter.net")
set(CTEST_DROP_LOCATION "/CDash/submit.php?project=freeDiameter")
set(CTEST_DROP_SITE_CDASH TRUE)
