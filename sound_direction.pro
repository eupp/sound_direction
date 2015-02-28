
TEMPLATE = subdirs

SUBDIRS += \
    trikSound \
    cli

cli.depends = trikSound
