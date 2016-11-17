from ROOT import *
from array import array

inputs="""REGION: EB HT_300
purity in SR:  0.973596217553 +/- 0.00119962021258
ALT purity in SR:  0.950298226528 +/- 0.00155609976443
MCALT purity in SR:  0.967925077841 +/- 0.00104850830371
--
REGION: EB HT_500
purity in SR:  0.984663025585 +/- 0.00178002259874
ALT purity in SR:  0.940906981849 +/- 0.0036791236455
MCALT purity in SR:  0.975866513872 +/- 0.00152227614807
--
REGION: EB HT_800
purity in SR:  0.990843201854 +/- 0.00305520670643
ALT purity in SR:  0.931138881833 +/- 0.0103584174658
MCALT purity in SR:  0.984349945369 +/- 0.00260299866091
--
REGION: EB HT_1500
purity in SR:  0.995785090759 +/- 0.00664084095953
ALT purity in SR:  0.684363289382 +/- 0.323331417765
MCALT purity in SR:  0.684363289382 +/- 0.323331417765
--
REGION: EE HT_300
purity in SR:  0.892260833447 +/- 0.00303010102456
ALT purity in SR:  0.855783734225 +/- 0.003346332091
MCALT purity in SR:  0.882273523211 +/- 0.00278009334999
--
REGION: EE HT_500
purity in SR:  0.933420152968 +/- 0.00493017781736
ALT purity in SR:  0.894348867885 +/- 0.00577200954817
MCALT purity in SR:  0.919503894718 +/- 0.00444529734854
--
REGION: EE HT_800
purity in SR:  0.962975638594 +/- 0.00928332815724
ALT purity in SR:  0.892197093785 +/- 0.0138048434012
MCALT purity in SR:  0.93606308689 +/- 0.00829130379482
--
REGION: EE HT_1500
purity in SR:  0.975229035086 +/- 0.029407655145
ALT purity in SR:  0.822896233892 +/- 0.388782113397
MCALT purity in SR:  0.961973456086 +/- 0.0263378542536
"""

x=[400,650,1150,2000]
xErr=[100,150,350,500]
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
nomEB.GetXaxis().SetTitle("H_{T} [GeV]")
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
