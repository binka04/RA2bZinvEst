from ROOT import *
from array import array

inputs="""
REGION: EB MHT_250
purity in SR:  0.974564570602 +/- 0.00135920388791
purity in SR:  0.963140380246 +/- 0.00156801038206
purity in SR:  0.981232498404 +/- 0.00124281808537
--
REGION: EB MHT_300
purity in SR:  0.979102097765 +/- 0.00150798290972
purity in SR:  0.949444177321 +/- 0.00216767029346
purity in SR:  0.978608169756 +/- 0.00152023310732
--
REGION: EB MHT_350
purity in SR:  0.983045225854 +/- 0.0014790739279
purity in SR:  0.916597613766 +/- 0.00336740494923
purity in SR:  0.987229963656 +/- 0.00135702799092
--
REGION: EB MHT_500
purity in SR:  0.990132424178 +/- 0.00242579214777
purity in SR:  0.951583772985 +/- 0.0054120086913
purity in SR:  0.992416329109 +/- 0.00220190346003
--
REGION: EE MHT_250
purity in SR:  0.894910224854 +/- 0.00344929066321
purity in SR:  0.881657388052 +/- 0.003593082598
purity in SR:  0.914431847355 +/- 0.00320219047026
--
REGION: EE MHT_300
purity in SR:  0.900365452582 +/- 0.0041804693019
purity in SR:  0.889575036213 +/- 0.00441074816921
purity in SR:  0.921336471498 +/- 0.00385127824097
--
REGION: EE MHT_350
purity in SR:  0.939076741739 +/- 0.00408715488612
purity in SR:  0.904229725947 +/- 0.00490450322945
purity in SR:  0.9593585471 +/- 0.00362515364511
--
REGION: EE MHT_500
purity in SR:  0.966696734637 +/- 0.00852230283342
purity in SR:  0.946815931576 +/- 0.0102245767225
purity in SR:  0.977796794444 +/- 0.00751037225409
"""

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

for i in range(len(EE["nom"])):
    EB["avg"].append((EB["nom"][i]+EB["mcalt"][i]+EB["alt"][i])/3.)
    EE["avg"].append((EE["nom"][i]+EE["mcalt"][i]+EE["alt"][i])/3.)

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

LUMItext = TText(600,1.01,"13 TeV (24.5/fb)")
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
