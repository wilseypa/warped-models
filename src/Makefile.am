INCLUDES=-I$(top_srcdir)/thirdparty

# The models are built with non-recursive make. This is faster and reduces
# Automake code duplication. To make keep everything more modular, the SOURCES
# and other macro definitions for each model are kept in `.am.stub` files in
# the models' directories. These stub files are included here directly.

include pingPong/Makefile.am.stub
include adaptTest/Makefile.am.stub
include phold/Makefile.am.stub
include raid/Makefile.am.stub
include rnd/Makefile.am.stub
include smmp/Makefile.am.stub
include epidemic/Makefile.am.stub
include circuitsimulationmodels/iscas89/Makefile.am.stub

noinst_PROGRAMS = $(PING_PONG_PROGRAM) $(UNIT_TEST_PROGRAM) $(ADAPT_TEST_PROGRAM) $(PHOLD_PROGRAM) $(RAID_PROGRAM) $(SMMP_PROGRAM) $(ISCAS89_PROGRAM) $(EPIDEMIC_PROGRAM)
