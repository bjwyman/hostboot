# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/usr/vpd/makefile $
#
# OpenPOWER HostBoot Project
#
# Contributors Listed Below - COPYRIGHT 2013,2014
# [+] International Business Machines Corp.
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied. See the License for the specific language governing
# permissions and limitations under the License.
#
# IBM_PROLOG_END_TAG
ROOTPATH = ../../..
MODULE = vpd

#include common objects between hb and runtime
include vpd.mk

#include unique objects
OBJS += vpd.o
OBJS += dimmPres.o
OBJS += rtvpd_load.o

SUBDIRS += test.d
SUBDIRS += runtime.d


BINARY_FILES = $(IMGDIR)/dimmspd.dat:3a9f53e3684e57401ed0aed75f25980fca99f40b
#Update to Murano DD2.1 for CCIN 54E6
BINARY_FILES += $(IMGDIR)/procmvpd.dat:c510eda768a9e3921ff957b549574507fd5c9ce9
BINARY_FILES += $(IMGDIR)/procmvpd_ven.dat:683bdae079c2e2708ffda655dbd849f14a16f816
#Update to Centaur DD2.0 for CCIN 31E8
BINARY_FILES += $(IMGDIR)/cvpd.dat:2e33352ca95279b992927336567adf960dca7d46

include ${ROOTPATH}/config.mk
