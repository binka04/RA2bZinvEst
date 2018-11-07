from os import environ
from ROOT import *
from math import sqrt
gROOT.ProcessLine(".L ~/tdrstyle.C")
gROOT.ProcessLine("setTDRStyle()")
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-r", "--region", dest="region", default="signal",
                  help="region -- signal, ldp, hdp -- only")
parser.add_option("-d", "--dir", dest="hash", default="",
                  help="hash code corresponding to your git commit or the directory on eos where your outputs were stored")
parser.add_option("-u", "--user", dest="user", default=environ.get('USER'),
                  help="cmslpc username for input files, default taken from env")
(options, args) = parser.parse_args()
assert( options.region == "signal" or options.region == "ldp" or options.region == "hdp" )
assert( options.hash != "" )

region = options.region
hash = options.hash
nBins = 0
MChistoFileName=""
MChistoTag=""
RzgHistoFileName=""
RzgHistoTag=""
trigWeightFileName=""
trigWeightTag=""
outputFileName=""

if region == "signal" :
    nBins = 46
    
    #MChistoFileName = "/eos/uscms/store/user/"+options.user+"/RA2bZinvEst/{0}/plotObs_photon_baseline.root".format(hash)
    MChistoFileName = "plotObs_photon_baseline.root".format(hash)
    MChistoTag = "AnalysisBins_BTag0_photon_baseline"
    #RzgHistoFileName = "/eos/uscms/store/user/"+options.user+"/RA2bZinvEst/{0}/RzGamma_PUweightOnly_signal_histo.root".format(hash)
    RzgHistoFileName = "RzGamma_PUweightOnly_signal_histo.root".format(hash)
    RzgHistoTag = "AnalysisBins_BTag0_RzGamma_signal"
    trigWeightFileName = "/eos/uscms/store/user/"+options.user+"/RA2bZinvEst/{0}/triggerUnc_signal_histo.root".format(hash)
    trigWeightTag = "AnalysisBins_BTag0_signal"
    fragmentationFileName = "../data/fragmentation.11022017_signal.txt"
    purityFileName = "../data/photonPurity_signal.txt"
    outputFileName = "gJets_signal.dat"

elif region == "ldp" : 
    nBins = 59
    #MChistoFileName = "/eos/uscms/store/user/"+options.user+"/RA2bZinvEst/{0}/plotObs_photonLDP_baseline.root".format(hash)
    MChistoFileName = "plotObs_photonLDP_baseline.root".format(hash)
    MChistoTag = "AnalysisBins_BTag0plusQCDCR_photonLDP_baseline"
    #RzgHistoFileName = "/eos/uscms/store/user/"+options.user+"/RA2bZinvEst/{0}/RzGamma_PUweightOnly_LDP_histo.root".format(hash)
    RzgHistoFileName = "RzGamma_PUweightOnly_LDP_histo.root".format(hash)
    RzgHistoTag = "AnalysisBins_BTag0plusQCDCR_RzGamma_LDP"
    trigWeightFileName = "/eos/uscms/store/user/"+options.user+"/RA2bZinvEst/{0}/triggerUnc_LDP_histo.root".format(hash)
    trigWeightTag = "AnalysisBins_BTag0plusQCDCR_RzGamma_LDP"
    fragmentationFileName = "../data/fragmentation.11022017_ldp.txt"
    purityFileName = "../data/photonPurity_QCD_CR.txt"
    outputFileName = "gJets_ldp.dat"
elif region == "hdp" :
    nBins = 59
    #MChistoFileName = "/eos/uscms/store/user/"+options.user+"/RA2bZinvEst/{0}/plotObs_photon_baseline.root".format(hash)
    MChistoFileName = "plotObs_photon_baseline.root".format(hash)
    MChistoTag = "AnalysisBins_BTag0plusQCDCR_photon_baseline"
    #RzgHistoFileName = "/eos/uscms/store/user/"+options.user+"/RA2bZinvEst/{0}/RzGamma_PUweightOnly_signal_histo.root".format(hash)
    RzgHistoFileName = "RzGamma_PUweightOnly_signal_histo.root".format(hash)
    RzgHistoTag = "AnalysisBins_BTag0plusQCDCR_RzGamma_signal"
    trigWeightFileName = "/eos/uscms/store/user/"+options.user+"/RA2bZinvEst/{0}/triggerUnc_signal_histo.root".format(hash)
    trigWeightTag = "AnalysisBins_BTag0plusQCDCR_RzGamma_signal"
    fragmentationFileName = "../data/fragmentation.11022017_hdp.txt"
    purityFileName = "../data/photonPurity_QCD_CR.txt"
    outputFileName = "gJets_hdp.dat"
else : 
    assert(0)

yieldInputFile = TFile(MChistoFileName,"READ")

GJetsEBHisto = yieldInputFile.Get(MChistoTag+"_EB_GJets")
GJetsEEHisto = yieldInputFile.Get(MChistoTag+"_EE_GJets")
GJetsHisto = yieldInputFile.Get(MChistoTag+"_GJets")
dataHisto = yieldInputFile.Get(MChistoTag+"_data")
dataEBHisto = yieldInputFile.Get(MChistoTag+"_EB_data")
dataEEHisto = yieldInputFile.Get(MChistoTag+"_EE_data")


if GJetsEBHisto.GetNbinsX() != GJetsEEHisto.GetNbinsX() : 
    print "number of bins in barrel and endcap don't match"
    assert(0)
if GJetsHisto.GetNbinsX() != GJetsEEHisto.GetNbinsX() : 
    print "number of bins in GJetsHisto and GJetsEEHisto don't match"
    assert(0)
if dataHisto.GetNbinsX() != GJetsEEHisto.GetNbinsX() :
    print "number of bins in dataHisto and GJetsEEHisto don't match"
    assert(0)
if dataEBHisto.GetNbinsX() != GJetsEEHisto.GetNbinsX() :
    print "number of bins in dataEBHisto and GJetsEEHisto don't match"
    assert(0)
if dataEEHisto.GetNbinsX() != GJetsEEHisto.GetNbinsX() :
    print "number of bins in dataEEHisto and GJetsEEHisto don't match"
    assert(0)

print "================ RzGamma =============="
ratioInputFile = TFile(RzgHistoFileName)
ZJetsHisto_Rzg = TH1F(ratioInputFile.Get(RzgHistoTag+"_ZJets"))
ZJetsHisto_Rzg.SetNameTitle("ZJetsHisto_Rzg","ZJetsHisto_Rzg")
GJetsHisto_Rzg = TH1F(ratioInputFile.Get(RzgHistoTag+"_GJets"))
GJetsHisto_Rzg.SetNameTitle("GJetsHisto_Rzg","GJetsHisto_Rzg")
RzGamma = TH1F(ZJetsHisto_Rzg)
print "ZJets:",RzGamma.GetBinContent(1)*36300./24500.
RzGamma.SetNameTitle("RzGamma","RzGamma")
RzGamma.Divide(GJetsHisto_Rzg)
print "GJets:",GJetsHisto_Rzg.GetBinContent(1)*36300./24500.
print "GJets/ZJets:",RzGamma.GetBinContent(1)
RzGamma.Scale(1./1.23)
print "RzG:",RzGamma.GetBinContent(1)

if RzGamma.GetNbinsX() != GJetsEEHisto.GetNbinsX() :
    print "number bins in RzGamma does not match others"
    assert(0)

for i in range(RzGamma.GetNbinsX()):
    print RzGamma.GetBinContent(i+1)
print "---------------------------------------"
print "================ TriggerWeights =============="
triggerWeight = [0.]*nBins
triggerWeightErr = [0.]*nBins

triggerInputFile = TFile(trigWeightFileName,"READ")
triggerWeightHisto = triggerInputFile.Get(trigWeightTag)
triggerWeightProf = triggerWeightHisto.ProfileX("triggerWeightProf",1,-1,"S")
 
assert(triggerWeightProf.GetNbinsX()==nBins)
for iBin in range(nBins):
    triggerWeight[iBin] = triggerWeightProf.GetBinContent(iBin+1)
    if triggerWeight[iBin] == 0. : 
        triggerWeight[iBin] = 1.
    triggerWeightErr[iBin] = triggerWeightProf.GetBinError(iBin+1)

print "triggerWeight: " 
print triggerWeight
print "triggerWeightErr"
print triggerWeightErr
print "------------------------------------------------------"

print "================ FRAGMENTATION FRACTION =============="
fragFrac = [0.]*nBins
fragFracErrUp = [0.]*nBins
fragFracErrDn = [0.]*nBins
fragFracFile = open(fragmentationFileName,"read")
for line in fragFracFile: 
    line = line[:-1]
    line_tokens = line.split(" ")
    while '' in line_tokens : 
        line_tokens.remove('')
    fragFrac[int(line_tokens[0])-1] = float(line_tokens[1])
    if( float(line_tokens[1]) == 1.0 ) :
        fragFracErrUp[int(line_tokens[0])-1] = 0.0
        fragFracErrDn[int(line_tokens[0])-1] = 0.03
    else : 
        fragFracErrUp[int(line_tokens[0])-1] = float(line_tokens[2])
        fragFracErrDn[int(line_tokens[0])-1] = float(line_tokens[3])

print fragFrac
print fragFracErrUp
print fragFracErrDn
print "------------------------------------------------------"

print "================= ID SCALE FACTORS ==================="
scaleFactorFile = TFile("../data/SFcorrections.Photons.Feb132017.root","READ")
scaleFactor = [scaleFactorFile.Get("h_inc").GetBinContent(1)]*nBins
scaleFactorErr = [scaleFactorFile.Get("h_inc").GetBinError(1)]*nBins
print scaleFactor
print scaleFactorErr
print "------------------------------------------------------"

print "================= PHOTON PURITY ==================="
purityEBAll=[]
purityEEAll=[]
purityEBerrAll=[]
purityEEerrAll=[]

purityInputFile = open(purityFileName,"r")
for line in purityInputFile : 
    if line[0] == "#" : continue
    line = line[:-1]
    dat = line.split()
    while '' in dat : 
        dat.remove('')
    purityEBAll.append(float(dat[0]))
    purityEBerrAll.append(float(dat[1]))
    purityEEAll.append(float(dat[2]))
    purityEEerrAll.append(float(dat[3]))

print "len(purityEB):",len(purityEBAll)
print "purity EB:",purityEBAll
print "len(purityEBerr):",len(purityEBerrAll)
print "purity EB error:",purityEBerrAll

print "len(purityEEerr):",len(purityEEerrAll)
print "purity EE:",purityEEAll
print "len(purityEEerr):",len(purityEEerrAll)
print "purity EE error:",purityEEerrAll
print "------------------------------------------------------"

DR=[0.0]*nBins
DRup=[0.0]*nBins
DRdown=[0.0]*nBins

outputDict = {}

outputDict["binIndex"]=[]
outputDict["nMCEBt"]=[]
outputDict["nMCECt"]=[]
outputDict["nMCGJ"]=[]
outputDict["nMCerr"]=[]
outputDict["Nobs"]=[]
outputDict["nEB"]=[]
outputDict["nEC"]=[]
outputDict["pEB"]=[]
outputDict["pEC"]=[]
outputDict["pEBerr"]=[]
outputDict["pECerr"]=[]
outputDict["SF"]=scaleFactor
outputDict["SFerr"]=[err/cv for cv,err in zip(scaleFactor,scaleFactorErr)]
outputDict["trigW"]=triggerWeight
outputDict["trigWsysErr"]=[0.01]*nBins
outputDict["trigWerr"]=[err/cv for cv,err in zip(triggerWeight,triggerWeightErr)]
outputDict["ZgR"] = []
outputDict["REr1"] = []
outputDict["f"]=fragFrac
outputDict["fsysErr"]=[0.005]*nBins
outputDict["ferrUp"]=[err/cv for cv,err in zip(fragFrac,fragFracErrUp)]
outputDict["ferrDn"]=[err/cv for cv,err in zip(fragFrac,fragFracErrDn)]
outputDict["purity"]=[]
outputDict["pErr"]=[]
outputDict["DR"]=[]
outputDict["DRup"]=[]
outputDict["DRlow"]=[]
outputDict["Yield"]=[]
outputDict["YstatUp"]=[]
outputDict["YstatLow"]=[]
outputDict["YsysUp"]=[]
outputDict["YsysLow"]=[]

poisZeroErr=1.67
for i in range(nBins) :
    outputDict["binIndex"].append(i+1)
    outputDict["nMCEBt"].append(GJetsEBHisto.GetBinContent(i+1)*scaleFactor[i])
    outputDict["nMCECt"].append(GJetsEEHisto.GetBinContent(i+1)*scaleFactor[i])
    outputDict["nMCGJ"].append(GJetsHisto.GetBinContent(i+1)*scaleFactor[i])
    if( outputDict["nMCGJ"][i] == 0 ) :
        outputDict["nMCerr"].append(sqrt(poisZeroErr*poisZeroErr+scaleFactorErr[i]*scaleFactorErr[i]/scaleFactor[i]/scaleFactor[i]))
    else:
        outputDict["nMCerr"].append(sqrt(GJetsHisto.GetBinError(i+1)*GJetsHisto.GetBinError(i+1)/outputDict["nMCGJ"][i]/outputDict["nMCGJ"][i]+scaleFactorErr[i]*scaleFactorErr[i]/scaleFactor[i]/scaleFactor[i]))
 #   outputDict["Nobs"].append(dataHisto.GetBinContent(i+1))
 #   outputDict["nEB"].append(dataEBHisto.GetBinContent(i+1))
    outputDict["Nobs"].append(GJetsHisto.GetBinContent(i+1))
    outputDict["nEB"].append(GJetsEBHisto.GetBinContent(i+1))
    outputDict["nEC"].append(GJetsEEHisto.GetBinContent(i+1))

    outputDict["pEB"].append(purityEBAll[i])
    outputDict["pEBerr"].append(purityEBerrAll[i]/outputDict["pEB"][i])
 #   outputDict["nEC"].append(dataEEHisto.GetBinContent(i+1))    
    outputDict["pEC"].append(purityEEAll[i])
    outputDict["pECerr"].append(purityEEerrAll[i]/outputDict["pEC"][i])
    if( RzGamma.GetBinContent(i+1) == 0. or RzGamma.GetBinContent(i+1) == 1. ):
        if i <= 30 : 
            outputDict["ZgR"].append(RzGamma.GetBinContent(i+1-10))
        else:
            outputDict["ZgR"].append(RzGamma.GetBinContent(i+1-8))                                   
        outputDict["REr1"].append(1.)
    else:
        outputDict["ZgR"].append(RzGamma.GetBinContent(i+1))#*scaleFactor[i])
        outputDict["REr1"].append(RzGamma.GetBinError(i+1)/outputDict["ZgR"][i])
    
    if( outputDict["nEB"][i] == 0 and outputDict["nEC"][i] == 0 ):
        outputDict["purity"].append(1.)
        outputDict["pErr"].append(0.)
    else:
        outputDict["purity"].append((outputDict["nEB"][i]*outputDict["pEB"][i]+outputDict["nEC"][i]*outputDict["pEC"][i])/(outputDict["nEB"][i]+outputDict["nEC"][i]))
        outputDict["pErr"].append((outputDict["pEBerr"][i]*outputDict["pEB"][i]*outputDict["nEB"][i]+outputDict["pECerr"][i]*outputDict["pEC"][i]*outputDict["nEC"][i])/(outputDict["pEB"][i]*outputDict["nEB"][i]+outputDict["pEC"][i]*outputDict["nEC"][i]))
    outputDict["DR"].append(1.0)
    outputDict["DRup"].append(0.000)
    outputDict["DRlow"].append(0.000)
    
   # outputDict["Yield"].append(outputDict["ZgR"][i]/outputDict["trigW"][i]/outputDict["SF"][i]*outputDict["f"][i]*(outputDict["nEB"][i]*outputDict["pEB"][i]+outputDict["nEC"][i]*outputDict["pEC"][i]))

    outputDict["Yield"].append(outputDict["ZgR"][i]*(outputDict["nEB"][i]+outputDict["nEC"][i]))

    if( outputDict["nEB"][i] == 0 and outputDict["nEC"][i] != 0 ):
        outputDict["YstatUp"].append(sqrt(poisZeroErr*poisZeroErr+outputDict["nEC"][i])/outputDict["Yield"][i])
        outputDict["YstatLow"].append(sqrt(poisZeroErr*poisZeroErr+outputDict["nEC"][i])/outputDict["Yield"][i])
    elif( outputDict["nEC"][i] == 0 and outputDict["nEB"][i] != 0 ):
        outputDict["YstatUp"].append(sqrt(outputDict["nEB"][i]+poisZeroErr*poisZeroErr)/outputDict["Yield"][i])
        outputDict["YstatLow"].append(sqrt(outputDict["nEB"][i]+poisZeroErr*poisZeroErr)/outputDict["Yield"][i])
    elif( outputDict["nEC"][i] == 0 and outputDict["nEB"][i] == 0 ):
        outputDict["YstatUp"].append(sqrt(2*poisZeroErr*poisZeroErr*outputDict["ZgR"][i]*outputDict["ZgR"][i]))
        outputDict["YstatLow"].append(sqrt(2*poisZeroErr*poisZeroErr*outputDict["ZgR"][i]*outputDict["ZgR"][i]))
    else : 
        outputDict["YstatUp"].append(sqrt(outputDict["nEB"][i]+outputDict["nEC"][i])/outputDict["Yield"][i])
        outputDict["YstatLow"].append(sqrt(outputDict["nEB"][i]+outputDict["nEC"][i])/outputDict["Yield"][i])
        
    outputDict["YsysUp"].append(sqrt(outputDict["REr1"][i]*outputDict["REr1"][i]+outputDict["pErr"][i]*outputDict["pErr"][i]+outputDict["ferrUp"][i]*outputDict["ferrUp"][i]+outputDict["trigWerr"][i]*outputDict["trigWerr"][i]+outputDict["trigWsysErr"][i]*outputDict["trigWsysErr"][i]+outputDict["fsysErr"][i]*outputDict["fsysErr"][i]+outputDict["SFerr"][i]*outputDict["SFerr"][i]))
    outputDict["YsysLow"].append(sqrt(outputDict["REr1"][i]*outputDict["REr1"][i]+outputDict["pErr"][i]*outputDict["pErr"][i]+outputDict["ferrDn"][i]*outputDict["ferrDn"][i]+outputDict["trigWerr"][i]*outputDict["trigWerr"][i]+outputDict["trigWsysErr"][i]*outputDict["trigWsysErr"][i]+outputDict["fsysErr"][i]*outputDict["fsysErr"][i]+outputDict["SFerr"][i]*outputDict["SFerr"][i]))

    #if (i == nBins):
    print " YIELD here :"
    print outputDict["Yield"]
    print "    "

    print " YIELD stat here :"
    print outputDict["YstatUp"]
    print "    "

    print " YIELD sys here  :"
    print outputDict["YsysUp"]
    print "    "

    print " nEC+nEB  :"
    print outputDict["nEC"]+outputDict["nEB"]
    print "    "


    print " nEC  :"
    print outputDict["nEC"]
    print "    "

columnNames=["binIndex","nMCGJ","nMCerr","nMCEBt","nMCECt","Nobs","nEB","pEB","pEBerr","nEC","pEC","pECerr","trigW","trigWsysErr","trigWerr","SF","SFerr","ZgR","REr1","f","fsysErr","ferrUp","ferrDn","purity","pErr","DR","DRup","DRlow","Yield","YstatUp","YstatLow","YsysUp","YsysLow"]





## Final table
outputFile = open(outputFileName,"w")
formattingString=" {0} : {1}( {2})|{3} |{4} |{5} |{6}| {7}({8}) |{9}| {10}({11}) | {12}({13},{14}) | {15}({16}) |{17}({18}) |{19}({20},+{21}-{22}) |{23}({24})| {25}(+{26}-{27})| {28}(+{29}-{30},+{31}-{32})"
outputFile.write(formattingString.format(*columnNames))
outputFile.write("\n")
for b in range(nBins):
    dataInBin = []
    for col in columnNames:
        if( isinstance(outputDict[col][b], int) ):
            #outputFile.write( col,"int")
            dataInBin.append(outputDict[col][b])
        elif( isinstance(outputDict[col][b], float) ):
            #outputFile.write( col,"float")
            dataInBin.append(round(outputDict[col][b],3))
        else:
            outputFile.write( "Something went wrong: ",type(outputDict[col][b]))
            assert(0)
    outputFile.write(formattingString.format(*dataInBin))
    outputFile.write("\n")
outputFile.close

can = TCanvas("can","can",500,500)
can.SetTopMargin(.08)
RzgCorr = TH1F("RzgCorr","RzgCorr",nBins,0.5,nBins+0.5)
for i in range(nBins):
    RzgCorr.SetBinContent(i+1,outputDict["ZgR"][i]/outputDict["trigW"][i]/outputDict["SF"][i])
    RzgCorr.SetBinError(i+1,sqrt(outputDict["REr1"][i]*outputDict["REr1"][i]+outputDict["trigWerr"][i]*outputDict["trigWerr"][i]+outputDict["SFerr"][i]*outputDict["SFerr"][i])*outputDict["ZgR"][i]/outputDict["trigW"][i]/outputDict["SF"][i])

RzgCorr.SetMarkerStyle(8)
RzgCorr.GetYaxis().SetRangeUser(0.,1.)
RzgCorr.GetYaxis().SetTitle("R_{Z/#gamma}")
RzgCorr.GetXaxis().SetTitle("i^{th} Bin")

RzgCorr.Draw("e1")

CMStext = TText(0.5,1.01,"CMS")
CMStext.SetTextFont(61)
CMStext.SetTextSize(0.045)
CMStext.Draw()

SIMtext = TText(7.,1.01,"simulation")
SIMtext.SetTextFont(52)
SIMtext.SetTextSize(0.045)
SIMtext.Draw()

LUMItext = TText(38,1.01,"13 TeV")
LUMItext.SetTextFont(51)
LUMItext.SetTextSize(0.045)
LUMItext.Draw()
