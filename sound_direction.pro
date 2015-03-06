
TEMPLATE = subdirs

SUBDIRS += \
    trikSound \
    cli \
    tests

cli.depends = trikSound
test.depends = trikSound
