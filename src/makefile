CC=g++
CFLAGS=
LIBS=`root-config --cflags --glibs` -L $(ROOTSYS)/lib
INCLUDES=-I$(CMSSW_BASE)/src/
EXECS=plotObs_baseline plotObs_GJetsCR_baseline plotRzGamma plotMinPhotonDR triggerEffInputs plotPurityProperties
$(EXECS):RA2bTree.cc definitions.h skimSamples.cc plotterUtils.cc 
plotObs_GJetsCR_baseline:plotObs_GJetsCR_baseline.cc
plotObs_baseline:plotObs_baseline.cc
plotRzGamma:plotRzGamma.cc CMS_lumi.h CMS_lumi.cc
plotMinPhotonDR:plotMinPhotonDR.cc bibhuDefinitions.h
triggerEffInputs:triggerEffInputs.cc
plotPurityProperties:plotPurityProperties.cc

all:$(EXECS)

plotPurityProperties:
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) plotPurityProperties.cc -o $@
plotMinPhotonDR:
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) plotMinPhotonDR.cc -o $@
triggerEffInputs:
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) triggerEffInputs.cc -o $@
plotRzGamma:
	$(CC) -g $(CFLAGS) $(LIBS) $(INCLUDES) plotRzGamma.cc -o $@
plotObs_baseline:
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) plotObs_baseline.cc -o $@

plotObs_GJetsCR_baseline:
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) plotObs_GJetsCR_baseline.cc -o $@

clean : 
	rm $(EXECS) 