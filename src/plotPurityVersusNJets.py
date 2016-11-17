from ROOT import *
from array import array

inputs="""REGION: EE NJets_1.5
purity in SR:  0.910529950258 +/- 0.00370840707641
ALT purity in SR:  0.875601155746 +/- 0.00416443141448
MCALT purity in SR:  0.898461585425 +/- 0.00342296657629
--
REGION: EE NJets_2.5
purity in SR:  0.897684599866 +/- 0.00453102125746
ALT purity in SR:  0.862107791422 +/- 0.00502693190116
MCALT purity in SR:  0.892056036722 +/- 0.00400385054975
--
REGION: EE NJets_3.5
purity in SR:  0.897854018223 +/- 0.00569170622824
ALT purity in SR:  0.849490650279 +/- 0.00644745690689
MCALT purity in SR:  0.878044934549 +/- 0.00534789227576
--
REGION: EE NJets_5.5
purity in SR:  0.910964929541 +/- 0.0159164019225
ALT purity in SR:  0.84249372466 +/- 0.0189912356491
MCALT purity in SR:  0.882226366376 +/- 0.0144254889561
--
REGION: EE NJets_7.5
purity in SR:  0.900197083594 +/- 0.0565285349974
ALT purity in SR:  0.857003758909 +/- 0.404896412033
MCALT purity in SR:  0.88833435663 +/- 0.0486206323477
--
REGION: EB NJets_1.5
purity in SR:  0.977733828642 +/- 0.00140845029621
ALT purity in SR:  0.948133931258 +/- 0.0020597419815
MCALT purity in SR:  0.970245098435 +/- 0.00121338586393
--
REGION: EB NJets_2.5
purity in SR:  0.977637193995 +/- 0.001702917281
ALT purity in SR:  0.944817177668 +/- 0.00254635464748
MCALT purity in SR:  0.969286048969 +/- 0.0015271851925
--
REGION: EB NJets_3.5
purity in SR:  0.974366441751 +/- 0.00223142823062
ALT purity in SR:  0.945170146048 +/- 0.00320689210207
MCALT purity in SR:  0.96967495369 +/- 0.00183685115206
--
REGION: EB NJets_5.5
purity in SR:  0.976693078732 +/- 0.00637340675199
ALT purity in SR:  0.953984947021 +/- 0.00871339737715
MCALT purity in SR:  0.976226604527 +/- 0.00457777505173
--
REGION: EB NJets_7.5
purity in SR:  0.814110817858 +/- 0.384631392478
ALT purity in SR:  0.68454155239 +/- 0.32341563916
MCALT purity in SR:  0.68454155239 +/- 0.32341563916"""

x=[2,3,4.5,6.5,9]
xErr=[.5,.5,1,1,1.5]
EB={"nom":[],"alt":[],"mcalt":[],"nomErr":[],"altErr":[],"mcaltErr":[]}
EE={"nom":[],"alt":[],"mcalt":[],"nomErr":[],"altErr":[],"mcaltErr":[]}
for measurement in inputs.split("--"):
    lines = measurement[1:].split("\n")
    #print lines
    if lines[0].find("EB") != -1 : 
        EB["nom"].append(float(lines[1].split(":  ")[1].split(" +/- ")[0]))
        EB["nomErr"].append(float(lines[1].split(":  ")[1].split(" +/- ")[1]))
        
    else :
        EE["nom"].append(float(lines[1].split(":  ")[1].split(" +/- ")[0]))
        EE["nomErr"].append(float(lines[1].split(":  ")[1].split(" +/- ")[1]))

#print "x:",x
#print "EB['nom']:",EB["nom"]
#print "EB['nomErr']:",EB["nomErr"]
#print "EE['nom']:",EE["nom"]
#print "EE['nomErr']:",EE["nomErr"]

gROOT.ProcessLine(".L ~/tdrstyle.C")
gROOT.ProcessLine("setTDRStyle()")

can = TCanvas("can","can",500,500)

nomEB = TGraphErrors(len(x),array("f",x),array("f",EB["nom"]),array("f",xErr),array("f",EB["nomErr"]))
nomEB.SetMarkerStyle(8)
nomEB.GetXaxis().SetTitle("n_{j}")
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
