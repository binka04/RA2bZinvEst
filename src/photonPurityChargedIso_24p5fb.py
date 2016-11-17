from ROOT import *
gROOT.SetBatch(True)
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-s", "--saveTag", dest="saveTag", default="inclusive",
                  help="tag added to output file names")
parser.add_option("-e", "--encap", dest="endcap", default=False,action="store_true",
                  help="fit endcap photons")
parser.add_option("-b", "--binLabel", dest="binLabel", default="MHT_200",
                  help="histo bin label")

(options, args) = parser.parse_args()

saveTag = options.saveTag
endcap = options.endcap
binLabel = options.binLabel
IsoChrg = RooRealVar("sieie","Iso_{chrg} [GeV]",0,20.)

EEorEB=""
if endcap : 
    EEorEB="EE"
else:
    EEorEB="EB"

inputFile = TFile("plotPurityProperties_photonLoose.root","READ")

#print "photonIsoChrgLowSieie_{0}_{1}_photonLoose_GJets".format(EEorEB,binLabel)
prompt_hist = inputFile.Get("photonIsoChrgLowSieie_{0}_{1}_photonLoose_GJets".format(EEorEB,binLabel))
#print "photonIsoChrgHighSieie_{0}_{1}_photonLoose_Data".format(EEorEB,binLabel)
nonPrompt_hist = inputFile.Get("photonIsoChrgHighSieie_{0}_{1}_photonLoose_data".format(EEorEB,binLabel))
#print "photonIsoChrgLowSieie_{0}_{1}_photonLoose_QCD".format(EEorEB,binLabel)
nonPromptMC_hist = inputFile.Get("photonIsoChrgLowSieie_{0}_{1}_photonLoose_QCD".format(EEorEB,binLabel))
#print "photonIsoChrgHighSieie_{0}_{1}_photonLoose_QCD".format(EEorEB,binLabel)
nonPromptMCALT_hist = inputFile.Get("photonIsoChrgHighSieie_{0}_{1}_photonLoose_QCD".format(EEorEB,binLabel))
#print "photonIsoChrgLowSieie_{0}_{1}_photonLoose_Data".format(EEorEB,binLabel)
Data_hist = inputFile.Get("photonIsoChrgLowSieie_{0}_{1}_photonLoose_data".format(EEorEB,binLabel))

prompt_hist.Scale(1./prompt_hist.Integral())
prompt_hist.SetFillColor(kCyan)
prompt_hist.SetLineWidth(2)
prompt_hist.SetLineColor(1)
prompt_hist.GetXaxis().SetTitle("Iso_{chrg} [GeV]")
prompt_hist.GetYaxis().SetTitle("Events")

nonPrompt_hist.Scale(1./nonPrompt_hist.Integral())
nonPrompt_hist.SetFillColor(kGray)
nonPrompt_hist.SetLineWidth(2)

nonPromptMC_hist.Scale(1./nonPromptMC_hist.Integral())
nonPromptMC_hist.SetFillColor(kGray)
nonPromptMC_hist.SetLineWidth(2)

Data_hist.SetMarkerStyle(8)

sieie = RooRealVar("sieie","sieie",0.,10.)

if endcap : 
    sieie.setRange("SR",0.0,1.79);
else :
    sieie.setRange("SR",0.0,2.67);

promptDataHist = RooDataHist("promptDataHist","promptDataHist",RooArgList(sieie),prompt_hist)
promptPdf = RooHistPdf("promptPdf","promptPdf",RooArgSet(sieie),promptDataHist)

nonpromptDataHist = RooDataHist("nonpromptDataHist","nonpromptDataHist",RooArgList(sieie),nonPrompt_hist)
nonpromptDataPdf = RooHistPdf("nonpromptDataPdf","nonpromptDataPdf",RooArgSet(sieie),nonpromptDataHist)

nonpromptHist = RooDataHist("nonpromptHist","nonpromptHist",RooArgList(sieie),nonPromptMC_hist)
nonpromptPdf = RooHistPdf("nonpromptPdf","nonpromptPdf",RooArgSet(sieie),nonpromptHist)

nonpromptALTHist = RooDataHist("nonpromptALTHist","nonpromptALTHist",RooArgList(sieie),nonPromptMCALT_hist)
nonpromptALTPdf = RooHistPdf("nonpromptALTPdf","nonpromptALTPdf",RooArgSet(sieie),nonpromptALTHist)

dataHist = RooDataHist("dataHist","dataHist",RooArgList(sieie),Data_hist)

frac = RooRealVar("frac","frac",.85,0.,1.)
fracALT = RooRealVar("fracALT","fracALT",.85,0.,1.)
fracMCALT = RooRealVar("fracMCALT","fracMCALT",.85,0.,1.)

model = RooAddPdf("model","model",promptPdf,nonpromptPdf,frac)
modelALT = RooAddPdf("modelALT","modelALT",promptPdf,nonpromptDataPdf,fracALT)
modelMCALT = RooAddPdf("modelALT","modelALT",promptPdf,nonpromptALTPdf,fracMCALT)

model.fitTo(dataHist)
modelALT.fitTo(dataHist)
modelMCALT.fitTo(dataHist)

gROOT.ProcessLine(".L ~/tdrstyle.C")
gROOT.ProcessLine("setTDRStyle()")

can = TCanvas("can","can",500,500)

fitPad = TPad("fit","fit",0.01,0.3,0.99,0.99)
fitPad.SetTopMargin(.1)
fitPad.Draw()
ratioPad = TPad("ratio","ratio",0.01,0.01,0.99,0.29)
ratioPad.SetBottomMargin(.07)
ratioPad.SetTopMargin(0.)
ratioPad.Draw()

fitPad.cd()
plot = sieie.frame()
dataHist.plotOn(plot)
model.plotOn(plot)
nonpromptPdf.plotOn(plot,RooFit.FillColor(2),RooFit.FillStyle(3144),RooFit.LineColor(2),RooFit.Normalization(1.0-frac.getVal()))
plot.GetXaxis().SetNdivisions(505)
plot.Draw()
ratioPad.cd()
ratio = TH1F(prompt_hist)
ratio.Scale(Data_hist.Integral()*frac.getVal()/prompt_hist.Integral())
ratio.Add(nonPromptMC_hist,float(Data_hist.Integral()*(1.-frac.getVal())/nonPromptMC_hist.Integral()))
ratio.Divide(Data_hist)
ratio.SetMarkerStyle(8)
ratio.GetYaxis().SetRangeUser(0.,3.)
ratio.GetXaxis().SetTitleOffset(1.5)
ratio.GetXaxis().SetTitleSize(1.2)
ratio.GetXaxis().SetLabelOffset(1.5)
ratio.GetXaxis().SetNdivisions(505)
ratio.GetYaxis().SetLabelSize(.1)
ratio.GetYaxis().SetTitleSize(.15)
ratio.GetYaxis().SetTitleOffset(.5)
ratio.GetYaxis().SetTitle("Model/Data")
ratio.Draw("e1")

canALT = TCanvas("canALT","canALT",500,500)

fitPadALT = TPad("fitALT","fitALT",0.01,0.3,0.99,0.99)
fitPadALT.SetTopMargin(.1)
fitPadALT.Draw()
ratioPadALT = TPad("ratioALT","ratioALT",0.01,0.01,0.99,0.29)
ratioPadALT.SetBottomMargin(.07)
ratioPadALT.SetTopMargin(0.)
ratioPadALT.Draw()

fitPadALT.Draw()
ratioPadALT.Draw()

fitPadALT.cd()
plotALT = sieie.frame()
dataHist.plotOn(plotALT)
modelALT.plotOn(plotALT)
nonpromptDataPdf.plotOn(plotALT,RooFit.FillColor(2),RooFit.FillStyle(3144),RooFit.LineColor(2),RooFit.Normalization(1.0-fracALT.getVal()))
plotALT.GetXaxis().SetNdivisions(505)
plotALT.Draw()
ratioPadALT.cd()
ratioALT = TH1F(prompt_hist)
ratioALT.Scale(Data_hist.Integral()*fracALT.getVal()/prompt_hist.Integral())
ratioALT.Add(nonPrompt_hist,float(Data_hist.Integral()*(1.-fracALT.getVal())/nonPrompt_hist.Integral()))
ratioALT.Divide(Data_hist)
ratioALT.SetMarkerStyle(8)
ratioALT.GetYaxis().SetRangeUser(0.,3.)
ratioALT.GetXaxis().SetTitleOffset(1.5)
ratioALT.GetXaxis().SetTitleSize(1.2)
ratioALT.GetXaxis().SetLabelOffset(1.5)
ratioALT.GetXaxis().SetNdivisions(505)
ratioALT.GetYaxis().SetLabelSize(.1)
ratioALT.GetYaxis().SetTitleSize(.15)
ratioALT.GetYaxis().SetTitleOffset(.5)
ratioALT.GetYaxis().SetTitle("Model/Data")
ratioALT.Draw("e1")

canMCALT = TCanvas("canMCALT","canMCALT",500,500)

fitPadMCALT = TPad("fitMCALT","fitMCALT",0.01,0.3,0.99,0.99)
fitPadMCALT.SetTopMargin(.1)
fitPadMCALT.Draw()
ratioPadMCALT = TPad("ratioMCALT","ratioMCALT",0.01,0.01,0.99,0.29)
ratioPadMCALT.SetBottomMargin(.07)
ratioPadMCALT.SetTopMargin(0.)
ratioPadMCALT.Draw()

fitPadMCALT.Draw()
ratioPadMCALT.Draw()

fitPadMCALT.cd()
plotMCALT = sieie.frame()
dataHist.plotOn(plotMCALT)
modelMCALT.plotOn(plotMCALT)
nonpromptDataPdf.plotOn(plotMCALT,RooFit.FillColor(2),RooFit.FillStyle(3144),RooFit.LineColor(2),RooFit.Normalization(1.0-fracMCALT.getVal()))
plotMCALT.GetXaxis().SetNdivisions(505)
plotMCALT.Draw()
ratioPadMCALT.cd()
ratioMCALT = TH1F(prompt_hist)
ratioMCALT.Scale(Data_hist.Integral()*fracMCALT.getVal()/prompt_hist.Integral())
ratioMCALT.Add(nonPrompt_hist,float(Data_hist.Integral()*(1.-fracMCALT.getVal())/nonPrompt_hist.Integral()))
ratioMCALT.Divide(Data_hist)
ratioMCALT.SetMarkerStyle(8)
ratioMCALT.GetYaxis().SetRangeUser(0.,3.)
ratioMCALT.GetXaxis().SetTitleOffset(1.5)
ratioMCALT.GetXaxis().SetTitleSize(1.2)
ratioMCALT.GetXaxis().SetLabelOffset(1.5)
ratioMCALT.GetXaxis().SetNdivisions(505)
ratioMCALT.GetYaxis().SetLabelSize(.1)
ratioMCALT.GetYaxis().SetTitleSize(.15)
ratioMCALT.GetYaxis().SetTitleOffset(.5)
ratioMCALT.GetYaxis().SetTitle("Model/Data")
ratioMCALT.Draw("e1")

promptIntegral   = promptPdf.createIntegral(RooArgSet(sieie));
promptSRIntegral = promptPdf.createIntegral(RooArgSet(sieie),"SR");
nonpromptIntegral = nonpromptPdf.createIntegral(RooArgSet(sieie));
nonpromptSRIntegral = nonpromptPdf.createIntegral(RooArgSet(sieie),"SR");
nonpromptDataIntegral = nonpromptDataPdf.createIntegral(RooArgSet(sieie));
nonpromptDataSRIntegral = nonpromptDataPdf.createIntegral(RooArgSet(sieie),"SR");
modelIntegral    = model.createIntegral(RooArgSet(sieie));
modelSRIntegral  = model.createIntegral(RooArgSet(sieie),"SR");

#print "promptALL:",promptIntegral.getVal()
#print "promptSR:",promptSRIntegral.getVal()
#print "nonpromptALL:",nonpromptIntegral.getVal()
#print "nonpromptSR:",nonpromptSRIntegral.getVal()
#print "nonpromptDataALL:",nonpromptDataIntegral.getVal()
#print "nonpromptDataSR:",nonpromptDataSRIntegral.getVal()
#print "modelALL:",modelIntegral.getVal()
#print "modelSR:",modelSRIntegral.getVal()

r_prompt = promptSRIntegral.getVal()/promptIntegral.getVal() 
r_nonprompt = nonpromptSRIntegral.getVal()/nonpromptIntegral.getVal()
r_nonpromptData = nonpromptDataSRIntegral.getVal()/nonpromptDataIntegral.getVal() 

print "REGION: {0} {1}".format(EEorEB,binLabel)

purity=frac.getVal()/( (1.-frac.getVal()) * r_nonprompt/r_prompt + frac.getVal() )
#print "purity from fit: ",frac.getVal(),"+/-",frac.getError()
print "purity in SR: ",purity,"+/-",frac.getError()/frac.getVal()*purity
purityALT = fracALT.getVal()/( (1.-fracALT.getVal()) * r_nonpromptData/r_prompt + fracALT.getVal() )
#print "ALT purity from fit: ",fracALT.getVal(),"+/-",fracALT.getError() 
print "ALT purity in SR: ",purityALT,"+/-",fracALT.getError()/fracALT.getVal()*purityALT
purityMCALT = fracMCALT.getVal()/( (1.-fracMCALT.getVal()) * r_nonpromptData/r_prompt + fracMCALT.getVal() )
#print "MCALT purity from fit: ",fracMCALT.getVal(),"+/-",fracMCALT.getError() 
print "MCALT purity in SR: ",purityMCALT,"+/-",fracMCALT.getError()/fracMCALT.getVal()*purityMCALT

can.SetTopMargin(.1)
canALT.SetTopMargin(.1)
canMCALT.SetTopMargin(.1)

cmsText = TPaveText(.17,.9,.47,.96,"NDC")
cmsText.SetFillColor(0)
cmsText.SetBorderSize(0)
cmsText.AddText("CMS Preliminary")

fitPad.cd()
cmsText.Draw()
fitPadALT.cd()
cmsText.Draw()
fitPadMCALT.cd()
cmsText.Draw()

lumiText = TPaveText(.72,.9,.97,.96,"NDC")
lumiText.SetFillColor(0)
lumiText.SetBorderSize(0)
lumiText.AddText("24.5 fb^{-1} (13 TeV)")

fitPad.cd()
lumiText.Draw()
fitPadALT.cd()
lumiText.Draw()
fitPadMCALT.cd()
lumiText.Draw()

can.SaveAs("purityFit_24p5fb_LDP_{0}_{1}.png".format(EEorEB,saveTag))
can.SaveAs("purityFit_24p5fb_LDP_{0}_{1}.eps".format(EEorEB,saveTag))
can.SaveAs("purityFit_24p5fb_LDP_{0}_{1}.pdf".format(EEorEB,saveTag))
canALT.SaveAs("purityFit_ALT_24p5fb_LDP_{0}_{1}.png".format(EEorEB,saveTag))
canALT.SaveAs("purityFit_ALT_24p5fb_LDP_{0}_{1}.eps".format(EEorEB,saveTag))
canALT.SaveAs("purityFit_ALT_24p5fb_LDP_{0}_{1}.pdf".format(EEorEB,saveTag))
canMCALT.SaveAs("purityFit_MC_ALT_24p5fb_LDP_{0}_{1}.png".format(EEorEB,saveTag))
canMCALT.SaveAs("purityFit_MC_ALT_24p5fb_LDP_{0}_{1}.eps".format(EEorEB,saveTag))
canMCALT.SaveAs("purityFit_MC_ALT_24p5fb_LDP_{0}_{1}.pdf".format(EEorEB,saveTag))

fitPad.SetLogy()
fitPadALT.SetLogy()
fitPadMCALT.SetLogy()

can.SaveAs("purityFit_24p5fb_LDP_{0}_{1}_LogY.png".format(EEorEB,saveTag))
can.SaveAs("purityFit_24p5fb_LDP_{0}_{1}_LogY.eps".format(EEorEB,saveTag))
can.SaveAs("purityFit_24p5fb_LDP_{0}_{1}_LogY.pdf".format(EEorEB,saveTag))
canALT.SaveAs("purityFit_ALT_24p5fb_LDP_{0}_{1}_LogY.png".format(EEorEB,saveTag))
canALT.SaveAs("purityFit_ALT_24p5fb_LDP_{0}_{1}_LogY.eps".format(EEorEB,saveTag))
canALT.SaveAs("purityFit_ALT_24p5fb_LDP_{0}_{1}_LogY.pdf".format(EEorEB,saveTag))
canMCALT.SaveAs("purityFit_MC_ALT_24p5fb_LDP_{0}_{1}_LogY.png".format(EEorEB,saveTag))
canMCALT.SaveAs("purityFit_MC_ALT_24p5fb_LDP_{0}_{1}_LogY.eps".format(EEorEB,saveTag))
canMCALT.SaveAs("purityFit_MC_ALT_24p5fb_LDP_{0}_{1}_LogY.pdf".format(EEorEB,saveTag))
