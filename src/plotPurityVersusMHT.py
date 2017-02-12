from ROOT import *
from array import array

inputs="""REGION: EB MHT_250
purity in SR:  0.973466096551 +/- 0.00162928782857
ALT purity in SR:  0.964239661421 +/- 0.00181237348774
MCALT purity in SR:  0.974027537126 +/- 0.00140917965426
--
REGION: EB MHT_300
purity in SR:  0.975509602818 +/- 0.00185361836017
ALT purity in SR:  0.954404697291 +/- 0.00241514144864
MCALT purity in SR:  0.969454094809 +/- 0.0016662321643
--
REGION: EB MHT_350
purity in SR:  0.981757748422 +/- 0.00174085375809
ALT purity in SR:  0.920524138301 +/- 0.0039385077422
MCALT purity in SR:  0.970036307774 +/- 0.00148638631716
--
REGION: EB MHT_500
purity in SR:  0.993224600585 +/- 0.00242050870638
ALT purity in SR:  0.882406918084 +/- 0.0172345202464
MCALT purity in SR:  0.98795565185 +/- 0.0021080832843
--
REGION: EE MHT_250
purity in SR:  0.88688631074 +/- 0.00394922194656
ALT purity in SR:  0.851776737779 +/- 0.00434835846051
MCALT purity in SR:  0.882042751545 +/- 0.00354908672516
--
REGION: EE MHT_300
purity in SR:  0.901156139894 +/- 0.00473781820932
ALT purity in SR:  0.861975494219 +/- 0.00529643063127
MCALT purity in SR:  0.888270558499 +/- 0.00436244361582
--
REGION: EE MHT_350
purity in SR:  0.933326559899 +/- 0.00473578878866
ALT purity in SR:  0.885993356208 +/- 0.00567414277755
MCALT purity in SR:  0.912317918029 +/- 0.00434135861779
--
REGION: EE MHT_500
purity in SR:  0.968586799591 +/- 0.00932934653271
ALT purity in SR:  0.882556239686 +/- 0.0158512855099
MCALT purity in SR:  0.942058620977 +/- 0.00787214238654
"""

x=[275,325,425,650]
xErr=[25,25,75,150]
EB={"nom":[0.957,0.957,0.949,0.952],"alt":[],"mcalt":[],"nomErr":[0.029,0.024,0.052,0.106],"altErr":[],"mcaltErr":[]}
EE={"nom":[0.854,0.877,0.904,0.926],"alt":[],"mcalt":[],"nomErr":[0.027,0.03,0.028,0.08],"altErr":[],"mcaltErr":[]}
# for measurement in inputs.split("--"):
#     lines = measurement[1:].split("\n")
#     #print lines
#     if lines[0].find("EB") != -1 : 
#         EB["nom"].append(float(lines[1].split(":  ")[1].split(" +/- ")[0]))
#         EB["nomErr"].append(float(lines[1].split(":  ")[1].split(" +/- ")[1]))
        
#     else :
#         EE["nom"].append(float(lines[1].split(":  ")[1].split(" +/- ")[0]))
#         EE["nomErr"].append(float(lines[1].split(":  ")[1].split(" +/- ")[1]))

#print "x:",x
#print "EB['nom']:",EB["nom"]
#print "EB['nomErr']:",EB["nomErr"]
#print "EE['nom']:",EE["nom"]
#print "EE['nomErr']:",EE["nomErr"]

gROOT.ProcessLine(".L ~/tdrstyle.C")
gROOT.ProcessLine("setTDRStyle()")

can = TCanvas("can","can",500,500)
can.SetTopMargin(0.08)

nomEB = TGraphErrors(len(x),array("f",x),array("f",EB["nom"]),array("f",xErr),array("f",EB["nomErr"]))
nomEB.SetMarkerStyle(8)
nomEB.GetXaxis().SetTitle("H_{T}^{miss} [GeV]")
nomEB.GetYaxis().SetRangeUser(.5,1.)
nomEB.GetYaxis().SetTitle("Photon Purity")

nomEE = TGraphErrors(len(x),array("f",x),array("f",EE["nom"]),array("f",xErr),array("f",EE["nomErr"]))
nomEE.SetMarkerStyle(4)

leg = TLegend(.2,.2,.5,.5)
leg.SetBorderSize(0)
leg.SetFillColor(0)
leg.AddEntry(nomEB,"EB","p")
leg.AddEntry(nomEE,"EE","p")

nomEB.Draw("Ap")
nomEE.Draw("p")
leg.Draw()

CMStext = TText(200,1.01,"CMS")
CMStext.SetTextFont(61)
CMStext.SetTextSize(0.045)
CMStext.Draw()

SIMtext = TText(285,1.01,"simulation")
SIMtext.SetTextFont(52)
SIMtext.SetTextSize(0.045)
SIMtext.Draw()

LUMItext = TText(600,1.01,"13 TeV (24.5/fb)")
LUMItext.SetTextFont(51)
LUMItext.SetTextSize(0.045)
LUMItext.Draw()

can.SaveAs("photonPurity_MHT.pdf")
can.SaveAs("photonPurity_MHT.png")
can.SaveAs("photonPurity_MHT.eps")
