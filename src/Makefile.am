INCLUDES=-I$(top_srcdir)/thirdparty

# The models are built with non-recursive make. This is faster and reduces
# Automake code duplication. To make keep everything more modular, the SOURCES
# and other macro definitions for each model are kept in `aminclude.am` files in
# the models' directories. These files are included here directly.

include traffic/aminclude.am
include phold/aminclude.am
include raid/aminclude.am
include smmp/aminclude.am
include epidemic/aminclude.am
include iscas/aminclude.am

noinst_PROGRAMS = $(TRAFFIC_PROGRAM) $(PHOLD_PROGRAM) $(RAID_PROGRAM) $(SMMP_PROGRAM) $(EPIDEMIC_PROGRAM) $(ISCAS_PROGRAM)
