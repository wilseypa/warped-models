INCLUDES=-I$(top_srcdir)/thirdparty

# The models are built with non-recursive make. This is faster and reduces
# Automake code duplication. To make keep everything more modular, the SOURCES
# and other macro definitions for each model are kept in `aminclude.am` files in
# the models' directories. These files are included here directly.

include pingPong/aminclude.am
include phold/aminclude.am
include raid/aminclude.am
include smmp/aminclude.am
include epidemic/aminclude.am
include circuitsimulationmodels/iscas89/aminclude.am

noinst_PROGRAMS = $(PING_PONG_PROGRAM) $(PHOLD_PROGRAM) $(RAID_PROGRAM) $(SMMP_PROGRAM) $(ISCAS89_PROGRAM) $(EPIDEMIC_PROGRAM)
