from ROOT import *
from math import sqrt

nBins = 46

yieldInputFile = TFile("plotObs_baseline.root","READ")

GJetsEBHisto = yieldInputFile.Get("AnalysisBins_BTag0_photon_baseline_EB_GJets")
GJetsEEHisto = yieldInputFile.Get("AnalysisBins_BTag0_photon_baseline_EE_GJets")
GJetsHisto = yieldInputFile.Get("AnalysisBins_BTag0_photon_baseline_GJets")
dataHisto = yieldInputFile.Get("AnalysisBins_BTag0_photon_baseline_data")
dataEBHisto = yieldInputFile.Get("AnalysisBins_BTag0_photon_baseline_EB_data")
dataEEHisto = yieldInputFile.Get("AnalysisBins_BTag0_photon_baseline_EE_data")

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

ratioInputFile = TFile("RzGamma_DR0p05_signal_histo.root")
ZJetsHisto = ratioInputFile.Get("AnalysisBins_BTag0_RzGamma_signal_ZJets")
GJetsHisto = ratioInputFile.Get("AnalysisBins_BTag0_RzGamma_signal_GJets")
RzGamma = TH1F(ZJetsHisto)
RzGamma.SetNameTitle("RzGamma","RzGamma")
RzGamma.Divide(GJetsHisto)
RzGamma.Scale(1./1.23)

if RzGamma.GetNbinsX() != GJetsEEHisto.GetNbinsX() :
    print "number bins in RzGamma does not match others"
    assert(0)

print "================ FRAGMENTATION FRACTION =============="
fragFrac = [0.]*nBins
fragFracErr = [0.]*nBins
fragFracFile = open("fragmentation.25112016.txt","read")
for line in fragFracFile: 
    line = line[:-1]
    line_tokens = line.split(" ")
    while '' in line_tokens : 
        line_tokens.remove('')
    fragFrac[int(line_tokens[0])-1] = float(line_tokens[1])
    fragFracErr[int(line_tokens[0])-1] = float(line_tokens[2])

print fragFrac
print fragFracErr
print "------------------------------------------------------"

print "================= ID SCALE FACTORS ==================="
scaleFactorFile = TFile("SFcorrections.Photons.root","READ")
scaleFactor = [scaleFactorFile.Get("h_inc").GetBinContent(1)]*nBins
scaleFactorErr = [scaleFactorFile.Get("h_inc").GetBinError(1)]*nBins
print scaleFactor
print scaleFactorErr
print "------------------------------------------------------"

NomPurityHTMHT_EB=[0.947065153182]*3
NomPurityHTMHT_EB.extend([0.956121166585]*3)
NomPurityHTMHT_EB.extend([0.984790438389]*4)

NomPurityHTMHT_EE=[0.877284301603]*3
NomPurityHTMHT_EE.extend([0.910163505329]*3)
NomPurityHTMHT_EE.extend([0.956462420031]*4)

DataPurityHTMHT_EB=[0.954562818866]*3
DataPurityHTMHT_EB.extend([0.920957814936]*3)
DataPurityHTMHT_EB.extend([0.885676319007]*4)

DataPurityHTMHT_EE=[0.863595295813]*3
DataPurityHTMHT_EE.extend([0.888194155599]*3)
DataPurityHTMHT_EE.extend([0.882485967811]*4)

MCaltPurityHTMHT_EB=[0.969584333786]*3
MCaltPurityHTMHT_EB.extend([0.970276710469]*3)
MCaltPurityHTMHT_EB.extend([0.986678278129]*4)

MCaltPurityHTMHT_EE=[0.889480018081]*3
MCaltPurityHTMHT_EE.extend([0.91319327327]*3)
MCaltPurityHTMHT_EE.extend([0.939235081042]*4)

purityEB=[]
purityEE=[]
purityEBerr=[]
purityEEerr=[]
print "Averaging 3 measurements for nominal"
print "Take half of the maximum variation as uncertainty"
for i in range(len(NomPurityHTMHT_EB)):
    purityEB.append(NomPurityHTMHT_EB[i]/3.+DataPurityHTMHT_EB[i]/3.+MCaltPurityHTMHT_EB[i]/3.)
    purityEE.append(NomPurityHTMHT_EE[i]/3.+DataPurityHTMHT_EE[i]/3.+MCaltPurityHTMHT_EE[i]/3.)
    purityEBerr.append(max([abs(NomPurityHTMHT_EB[i]-DataPurityHTMHT_EB[i]),abs(NomPurityHTMHT_EB[i]-MCaltPurityHTMHT_EB[i]),abs(DataPurityHTMHT_EB[i]-MCaltPurityHTMHT_EB[i])]))
    purityEEerr.append(max([abs(NomPurityHTMHT_EE[i]-DataPurityHTMHT_EE[i]),abs(NomPurityHTMHT_EE[i]-MCaltPurityHTMHT_EE[i]),abs(DataPurityHTMHT_EE[i]-MCaltPurityHTMHT_EE[i])]))

purityEBNJet7=list(purityEB)
purityEBNJet7.pop(4)
purityEBNJet7.pop(1)
purityEBAll = []
purityEBAll.extend(purityEB)
purityEBAll.extend(purityEB)
purityEBAll.extend(purityEB)
purityEBAll.extend(purityEBNJet7)
purityEBAll.extend(purityEBNJet7)

purityEBerrNJet7=list(purityEBerr)
purityEBerrNJet7.pop(4)
purityEBerrNJet7.pop(1)
purityEBerrAll=[]
purityEBerrAll.extend(purityEBerr)
purityEBerrAll.extend(purityEBerr)
purityEBerrAll.extend(purityEBerr)
purityEBerrAll.extend(purityEBerrNJet7)
purityEBerrAll.extend(purityEBerrNJet7)

purityEENJet7=list(purityEE)
purityEENJet7.pop(4)
purityEENJet7.pop(1)
purityEEAll=[]
purityEEAll.extend(purityEE)
purityEEAll.extend(purityEE)
purityEEAll.extend(purityEE)
purityEEAll.extend(purityEENJet7)
purityEEAll.extend(purityEENJet7)

purityEEerrNJet7=list(purityEEerr)
purityEEerrNJet7.pop(4)
purityEEerrNJet7.pop(1)
purityEEerrAll=[]
purityEEerrAll.extend(purityEEerr)
purityEEerrAll.extend(purityEEerr)
purityEEerrAll.extend(purityEEerr)
purityEEerrAll.extend(purityEEerrNJet7)
purityEEerrAll.extend(purityEEerrNJet7)

print "len(purityEB):",len(purityEBAll)
print "purity EB:",purityEBAll
print "len(purityEBerr):",len(purityEBerrAll)
print "purity EB error:",purityEBerrAll

print "len(purityEEerr):",len(purityEEerrAll)
print "purity EE:",purityEEAll
print "len(purityEEerr):",len(purityEEerrAll)
print "purity EE error:",purityEEerrAll

DR=[0.0]*nBins
DRup=[0.0]*nBins
DRdown=[0.0]*nBins

outputDict = {}

print "Note there are no trigger weights applied here!!"
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
outputDict["ZgR"] = []
outputDict["REr1"] = []
outputDict["RErUp"] = []
outputDict["RErLow"] = []
outputDict["f"] = []
outputDict["ferr"] = []
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
    outputDict["nMCEBt"].append(GJetsEBHisto.GetBinContent(i+1))
    outputDict["nMCECt"].append(GJetsEEHisto.GetBinContent(i+1))
    outputDict["nMCGJ"].append(GJetsHisto.GetBinContent(i+1)*scaleFactor[i])
    if( outputDict["nMCGJ"][i] == 0 ) :
        outputDict["nMCerr"].append(sqrt(poisZeroErr*poisZeroErr+scaleFactorErr[i]*scaleFactorErr[i]/scaleFactor[i]/scaleFactor[i]))
    else:
        outputDict["nMCerr"].append(sqrt(GJetsHisto.GetBinError(i+1)*GJetsHisto.GetBinError(i+1)/outputDict["nMCGJ"][i]/outputDict["nMCGJ"][i]+scaleFactorErr[i]*scaleFactorErr[i]/scaleFactor[i]/scaleFactor[i]))
    outputDict["Nobs"].append(dataHisto.GetBinContent(i+1))
    outputDict["nEB"].append(dataEBHisto.GetBinContent(i+1))
    outputDict["pEB"].append(purityEBAll[i])
    outputDict["pEBerr"].append(purityEBerrAll[i]/outputDict["pEB"][i])
    outputDict["nEC"].append(dataEEHisto.GetBinContent(i+1))    
    outputDict["pEC"].append(purityEEAll[i])
    outputDict["pECerr"].append(purityEEerrAll[i]/outputDict["pEC"][i])
    if( RzGamma.GetBinContent(i+1) == 0. or RzGamma.GetBinContent(i+1) == 1. ):
        if i <= 30 : 
            outputDict["ZgR"].append(RzGamma.GetBinContent(i+1-10))
        else:
            outputDict["ZgR"].append(RzGamma.GetBinContent(i+1-8))                                   
        outputDict["REr1"].append(1.)
    else:
        outputDict["ZgR"].append(RzGamma.GetBinContent(i+1))
        outputDict["REr1"].append(RzGamma.GetBinError(i+1)/outputDict["ZgR"][i])
    outputDict["RErUp"].append(0.053)
    outputDict["RErLow"].append(0.048)
    outputDict["f"].append(fragFrac[i])
    outputDict["ferr"].append(fragFracErr[i])
   
    #print "pEBerr[",i,"]:",outputDict["pEBerr"][i]
    #print "pEB[",i,"]:",outputDict["pEB"][i]
    #print "nEB[",i,"]:",outputDict["nEB"][i]
    #print "pECerr[",i,"]:",outputDict["pECerr"][i]
    #print "pEC[",i,"]:",outputDict["pEC"][i]
    #print "nEC[",i,"]:",outputDict["nEC"][i]

    #print "num:",sqrt(outputDict["pEBerr"][i]*outputDict["pEBerr"][i]*outputDict["pEB"][i]*outputDict["pEB"][i]*outputDict["nEB"][i]*outputDict["nEB"][i]+outputDict["pECerr"][i]*outputDict["pECerr"][i]*outputDict["pEC"][i]*outputDict["pEC"][i]*outputDict["nEC"][i]*outputDict["nEC"][i])
    #print "denom:",outputDict["pEB"][i]*outputDict["nEB"][i]+outputDict["pEC"][i]*outputDict["nEC"][i]
    
    if( outputDict["nEB"][i] == 0 and outputDict["nEC"][i] == 0 ):
        outputDict["purity"].append(1.)
        outputDict["pErr"].append(0.)
    else:
        outputDict["purity"].append((outputDict["nEB"][i]*outputDict["pEB"][i]+outputDict["nEC"][i]*outputDict["pEC"][i])/(outputDict["nEB"][i]+outputDict["nEC"][i]))
        outputDict["pErr"].append((outputDict["pEBerr"][i]*outputDict["pEB"][i]*outputDict["nEB"][i]+outputDict["pECerr"][i]*outputDict["pEC"][i]*outputDict["nEC"][i])/(outputDict["pEB"][i]*outputDict["nEB"][i]+outputDict["pEC"][i]*outputDict["nEC"][i]))
    outputDict["DR"].append(1.0)
    outputDict["DRup"].append(0.000)
    outputDict["DRlow"].append(0.000)
    
    outputDict["Yield"].append(outputDict["ZgR"][i]*(outputDict["nEB"][i]*outputDict["pEB"][i]+outputDict["nEC"][i]*outputDict["pEC"][i])*outputDict["f"][i])
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
        
    outputDict["YsysUp"].append(sqrt(outputDict["REr1"][i]*outputDict["REr1"][i]+outputDict["RErUp"][i]*outputDict["RErUp"][i]+outputDict["pErr"][i]*outputDict["pErr"][i]))
    outputDict["YsysLow"].append(sqrt(outputDict["REr1"][i]*outputDict["REr1"][i]+outputDict["RErLow"][i]*outputDict["RErLow"][i]+outputDict["pErr"][i]*outputDict["pErr"][i]))

columnNames=["binIndex","nMCGJ","nMCerr","nMCEBt","nMCECt","Nobs","nEB","pEB","pEBerr","nEC","pEC","pECerr","ZgR","REr1","RErUp","RErLow","f","ferr","purity","pErr","DR","DRup","DRlow","Yield","YstatUp","YstatLow","YsysUp","YsysLow"]
## Final table
outputFile = open("gJets_signal.dat","w")
formattingString=" {0} : {1}( {2})|{3} |{4} |{5} |{6}| {7}({8}) |{9}| {10}({11}) |{12}({13},+{14}-{15}) |{16}({17}) |{18}({19})| {20}(+{21}-{22})| {23}(+{24}-{25},+{26}-{27})"
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
