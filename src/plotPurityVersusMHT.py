from ROOT import *
from array import array

#purity_fb6795fa13173ba165a794d7c5e9ee8e93864edd.log
inputs="""
REGION: EB MHT_250
purity in SR:  0.970072782179 +/- 0.00136602160239
purity in SR:  0.96222903804 +/- 0.00149095022757
purity in SR:  0.97787263305 +/- 0.00125217306914
--
REGION: EB MHT_300
purity in SR:  0.97523036316 +/- 0.00150818460738
purity in SR:  0.963595415959 +/- 0.00172326615353
purity in SR:  0.974449800538 +/- 0.00152582602709
--
REGION: EB MHT_350
purity in SR:  0.980033307476 +/- 0.00143769914999
purity in SR:  0.967815378706 +/- 0.00172267690574
purity in SR:  0.985009747136 +/- 0.00132221247893
--
REGION: EB MHT_500
purity in SR:  0.988211430578 +/- 0.00235128301394
purity in SR:  0.980910541007 +/- 0.0028044913712
purity in SR:  0.991095137429 +/- 0.00213863122215
--
REGION: EE MHT_250
purity in SR:  0.915912882256 +/- 0.00591356013738
purity in SR:  0.909699599595 +/- 0.00602921647167
purity in SR:  0.931123616469 +/- 0.00550656371304
--
REGION: EE MHT_300
purity in SR:  0.910329788185 +/- 0.00742733973238
purity in SR:  0.903858936754 +/- 0.0076629447208
purity in SR:  0.930375175186 +/- 0.00680518379369
--
REGION: EE MHT_350
purity in SR:  0.946772967647 +/- 0.00670336891048
purity in SR:  0.900207277712 +/- 0.00874839571211
purity in SR:  0.963996255512 +/- 0.00600308165446
--
REGION: EE MHT_500
purity in SR:  0.980177080456 +/- 0.0106777356102
purity in SR:  0.961787485556 +/- 0.0140432356269
purity in SR:  0.986845268989 +/- 0.0094294928825
"""

f= open("Purity_Output_FINAL.txt","a")
f.write("#purity EB:      purity EB error:      purity EE:      purity EE error:\n")

x=[275,325,425,650]
xErr=[25,25,75,150]
EB={"nom":[],"alt":[],"mcalt":[],"nomErr":[],"altErr":[],"mcaltErr":[]}
EE={"nom":[],"alt":[],"mcalt":[],"nomErr":[],"altErr":[],"mcaltErr":[]}
for measurement in inputs.split("--"):
    lines = measurement[1:].split("\n")
    print lines
    if lines[0].find("EB") != -1 : 
        EB["alt"].append(float(lines[1].split(":  ")[1].split(" +/- ")[0]))
        EB["altErr"].append(float(lines[1].split(":  ")[1].split(" +/- ")[1]))
        EB["nom"].append(float(lines[2].split(":  ")[1].split(" +/- ")[0]))
        EB["nomErr"].append(float(lines[2].split(":  ")[1].split(" +/- ")[1]))
        EB["mcalt"].append(float(lines[3].split(":  ")[1].split(" +/- ")[0]))
        EB["mcaltErr"].append(float(lines[3].split(":  ")[1].split(" +/- ")[1]))
        
    else :
        EE["alt"].append(float(lines[1].split(":  ")[1].split(" +/- ")[0]))
        EE["altErr"].append(float(lines[1].split(":  ")[1].split(" +/- ")[1]))
        EE["nom"].append(float(lines[2].split(":  ")[1].split(" +/- ")[0]))
        EE["nomErr"].append(float(lines[2].split(":  ")[1].split(" +/- ")[1]))
        EE["mcalt"].append(float(lines[3].split(":  ")[1].split(" +/- ")[0]))
        EE["mcaltErr"].append(float(lines[3].split(":  ")[1].split(" +/- ")[1]))

EB["avg"] = []
EB["avgErr"] = []
EE["avg"] = []
EE["avgErr"] = []

f.write("#purity EB:       purity EE: \n")

for i in range(len(EE["nom"])):
    EB["avg"].append((EB["nom"][i]+EB["mcalt"][i]+EB["alt"][i])/3.)
    EE["avg"].append((EE["nom"][i]+EE["mcalt"][i]+EE["alt"][i])/3.)
    f.write(str(EB["avg"][i])+"    "+str(EE["avg"][i])+"\n")

f.write("#purity EB error:       purity EE error: \n")
for i in range(len(EE["nom"])):
    EB["avgErr"].append(0.)
    EE["avgErr"].append(0.)
    if( abs(EB["avg"][i] - EB["nom"][i]) > EB["avgErr"][i] ):  
        EB["avgErr"][i]=abs(EB["avg"][i] - EB["nom"][i])
    if( abs(EB["avg"][i] - EB["alt"][i]) > EB["avgErr"][i] ):  
        EB["avgErr"][i]=abs(EB["avg"][i] - EB["alt"][i])
    if( abs(EB["avg"][i] - EB["mcalt"][i]) > EB["avgErr"][i] ):  
        EB["avgErr"][i]=abs(EB["avg"][i] - EB["mcalt"][i])

    if( abs(EE["avg"][i] - EE["nom"][i]) > EE["avgErr"][i] ):  
        EE["avgErr"][i]=abs(EE["avg"][i] - EE["nom"][i])
    if( abs(EE["avg"][i] - EE["alt"][i]) > EE["avgErr"][i] ):  
        EE["avgErr"][i]=abs(EE["avg"][i] - EE["alt"][i])
    if( abs(EE["avg"][i] - EE["mcalt"][i]) > EE["avgErr"][i] ):  
        EE["avgErr"][i]=abs(EE["avg"][i] - EE["mcalt"][i])
    f.write(str(EB["avgErr"][i])+"    "+str(EE["avgErr"][i])+"\n")

print "x:",x
print "EB['nom']:",EB["nom"]
print "EB['nomErr']:",EB["nomErr"]
print "EB['mcalt']:",EB["mcalt"]
print "EB['mcaltErr']:",EB["mcaltErr"]
print "EB['alt']:",EB["alt"]
print "EB['altErr']:",EB["altErr"]

print "EE['nom']:",EE["nom"]
print "EE['nomErr']:",EE["nomErr"]
print "EE['mcalt']:",EE["mcalt"]
print "EE['mcaltErr']:",EE["mcaltErr"]
print "EE['alt']:",EE["alt"]
print "EE['altErr']:",EE["altErr"]

print "---------------------------------"
print "---------------------------------"
print "EB['avg']",EB["avg"]
print "EB['avgErr']",EB["avgErr"]
print "EE['avg']",EE["avg"]
print "EE['avgErr']",EE["avgErr"]

#f.write(str(EB["avg"])+"    "+str(EB["avgErr"])+"    "+str(EE["avg"])+"    "+str(EE["avgErr"])+ "\n")

gROOT.ProcessLine(".L ~/tdrstyle.C")
gROOT.ProcessLine("setTDRStyle()")

can = TCanvas("can","can",500,500)
can.SetTopMargin(0.08)

avgEB = TGraphErrors(len(x),array("f",x),array("f",EB["avg"]),array("f",xErr),array("f",EB["avgErr"]))
avgEB.SetMarkerStyle(8)
avgEB.GetXaxis().SetTitle("H_{T}^{miss} [GeV]")
avgEB.GetYaxis().SetRangeUser(.5,1.)
avgEB.GetYaxis().SetTitle("Photon Purity")

avgEE = TGraphErrors(len(x),array("f",x),array("f",EE["avg"]),array("f",xErr),array("f",EE["avgErr"]))
avgEE.SetMarkerStyle(4)

# - - - - - - - -
nomEB = TGraphErrors(len(x),array("f",x),array("f",EB["nom"]),array("f",xErr),array("f",EB["nomErr"]))
nomEB.SetMarkerStyle(8)
nomEB.GetXaxis().SetTitle("H_{T}^{miss} [GeV]")
nomEB.GetYaxis().SetRangeUser(.5,1.)
nomEB.GetYaxis().SetTitle("Photon Purity")

nomEE = TGraphErrors(len(x),array("f",x),array("f",EE["nom"]),array("f",xErr),array("f",EE["nomErr"]))
nomEE.SetMarkerStyle(4)

# - - - - - - - - 

altEB = TGraphErrors(len(x),array("f",x),array("f",EB["alt"]),array("f",xErr),array("f",EB["altErr"]))
altEB.SetMarkerStyle(8)
altEB.SetMarkerColor(2)
altEB.SetLineColor(2)
altEB.GetXaxis().SetTitle("H_{T}^{miss} [GeV]")
altEB.GetYaxis().SetRangeUser(.5,1.)
altEB.GetYaxis().SetTitle("Photon Purity")

altEE = TGraphErrors(len(x),array("f",x),array("f",EE["alt"]),array("f",xErr),array("f",EE["altErr"]))
altEE.SetMarkerStyle(4)
altEE.SetMarkerColor(2)
altEE.SetLineColor(2)

# - - - - - - - - 

mcaltEB = TGraphErrors(len(x),array("f",x),array("f",EB["mcalt"]),array("f",xErr),array("f",EB["mcaltErr"]))
mcaltEB.SetMarkerStyle(8)
mcaltEB.SetMarkerColor(4)
mcaltEB.SetLineColor(4)
mcaltEB.GetXaxis().SetTitle("H_{T}^{miss} [GeV]")
mcaltEB.GetYaxis().SetRangeUser(.5,1.)
mcaltEB.GetYaxis().SetTitle("Photon Purity")

mcaltEE = TGraphErrors(len(x),array("f",x),array("f",EE["mcalt"]),array("f",xErr),array("f",EE["mcaltErr"]))
mcaltEE.SetMarkerStyle(4)
mcaltEE.SetMarkerColor(4)
mcaltEE.SetLineColor(4)

# - - - - - - - - 

leg = TLegend(.2,.2,.5,.5)
leg.SetBorderSize(0)
leg.SetFillColor(0)
leg.AddEntry(nomEB,"EB","p")
leg.AddEntry(nomEE,"EE","p")
leg.AddEntry(altEB,"altEB","p")
leg.AddEntry(altEE,"altEE","p")
leg.AddEntry(mcaltEB,"mcaltEB","p")
leg.AddEntry(mcaltEE,"mcaltEE","p")

nomEB.Draw("Ap")
nomEE.Draw("p")
altEB.Draw("p")
altEE.Draw("p")
mcaltEB.Draw("p")
mcaltEE.Draw("p")
leg.Draw()

CMStext = TText(200,1.01,"CMS")
CMStext.SetTextFont(61)
CMStext.SetTextSize(0.045)
CMStext.Draw()

SIMtext = TText(285,1.01,"preliminary")
SIMtext.SetTextFont(52)
SIMtext.SetTextSize(0.045)
SIMtext.Draw()

LUMItext = TText(600,1.01,"13 TeV (35.9/fb)")
LUMItext.SetTextFont(51)
LUMItext.SetTextSize(0.045)
LUMItext.Draw()

can.SaveAs("../plots/purityResults/photonPurity_ALL_MHT.pdf")
can.SaveAs("../plots/purityResults/photonPurity_ALL_MHT.png")
can.SaveAs("../plots/purityResults/photonPurity_ALL_MHT.eps")

# - - - - - - - - 
canAvg = TCanvas("canAvg","canAvg",500,500)
canAvg.SetTopMargin(0.08)

legAvg = TLegend(.2,.2,.5,.5)
legAvg.SetBorderSize(0)
legAvg.SetFillColor(0)
legAvg.AddEntry(nomEB,"EB","p")
legAvg.AddEntry(nomEE,"EE","p")

avgEB.Draw("Ap")
avgEE.Draw("p")
leg.Draw()

CMStext.Draw()
SIMtext.Draw()
LUMItext.Draw()

canAvg.SaveAs("../plots/purityResults/photonPurity_MHT.pdf")
canAvg.SaveAs("../plots/purityResults/photonPurity_MHT.png")
canAvg.SaveAs("../plots/purityResults/photonPurity_MHT.eps")
