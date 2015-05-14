
TEMPLATE = subdirs

SUBDIRS += \
    trikSound \
    cli \
    tests \
    benchmarks

cli.depends = trikSound
test.depends = trikSound
