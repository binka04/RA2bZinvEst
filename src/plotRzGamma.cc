#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TChain.h"
#include "TH1F.h"
#include "TROOT.h"
#include "THStack.h"
#include "TPad.h"

#include <vector>
#include <map>
#include <iostream>

#include "CMS_lumi.cc"

#include "plotterUtils.cc"
#include "skimSamples.cc"
#include "definitions.h"
#include "RA2bTree.cc"

using namespace std;

int main(int argc, char** argv){

  if( argc != 2 ){ 
    cout << "1 argument needed:" << endl;
    cout << "0 - signal (single-photon) region will be used" << endl;
    cout << "1 - LDP (single-photon - LDP) region will be used" << endl;
    return 1;
  }
  int region = atoi(argv[1]);
  if( region < 0 || region > 1 ){
    cout << "argument 1 doesn't have an acceptible value" << endl;
    return 1;
  }

  TString regionNames[2] = {"signal","LDP"};

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle()");

  typedef plot<RA2bTree> plot;
  
  lumi = 24500;

  vector<TString> xlabels;
  xlabels.push_back("MHT [GeV]");
  xlabels.push_back("H_{T} [GeV]");
  xlabels.push_back("n_{j}");
  xlabels.push_back("n_{b}");
  xlabels.push_back("i^th Bin");
  xlabels.push_back("#Delta#Phi_{1}");
  xlabels.push_back("#Delta#Phi_{2}");
  xlabels.push_back("#Delta#Phi_{3}");
  xlabels.push_back("#Delta#Phi_{4}");

  plot MHTplot(*fillMHT<RA2bTree>,"MHT_RzGamma_"+regionNames[region],"H_{T}^{miss} [GeV]",15,300.,1800.);
  plot HTplot(*fillHT<RA2bTree>,"HT_RzGamma_"+regionNames[region],"H_{T} [GeV]",15,300,2800.);
  plot NJetsplot(*fillNJets<RA2bTree>,"NJets_RzGamma_"+regionNames[region],"n_{j}",14,1.5,15.5);
  plot BTagsplot(*fillBTags<RA2bTree>,"BTags_RzGamma_"+regionNames[region],"n_{b}",6,-0.5,5.5);

  plot Binsplot(*fillRA2b160Bins<RA2bTree>,"AnalysisBins_RzGamma_"+regionNames[region],"i^th Bin",160,0.5,160.5);
  
  plot DeltaPhi1plot(*fillDeltaPhi1<RA2bTree>,"DeltaPhi1_RzGamma_"+regionNames[region],"#Delta#Phi_{1}",20,0,3.1415);
  plot DeltaPhi2plot(*fillDeltaPhi2<RA2bTree>,"DeltaPhi2_RzGamma_"+regionNames[region],"#Delta#Phi_{2}",20,0,3.1415);
  plot DeltaPhi3plot(*fillDeltaPhi3<RA2bTree>,"DeltaPhi3_RzGamma_"+regionNames[region],"#Delta#Phi_{3}",20,0,3.1415);
  plot DeltaPhi4plot(*fillDeltaPhi4<RA2bTree>,"DeltaPhi4_RzGamma_"+regionNames[region],"#Delta#Phi_{4}",20,0,3.1415);

  vector<plot> plots;
  plots.push_back(MHTplot);
  plots.push_back(HTplot);
  plots.push_back(NJetsplot);
  plots.push_back(BTagsplot);
  plots.push_back(Binsplot);
  plots.push_back(DeltaPhi1plot);
  plots.push_back(DeltaPhi2plot);
  plots.push_back(DeltaPhi3plot);
  plots.push_back(DeltaPhi4plot);


  // background MC samples
  vector<RA2bTree*> samples;
  vector<TString> sampleNames;

  TString skimType;
  if( region == 0 )
    skimType="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV10/tree_signal/";
  if( region == 1 )
    skimType="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV10/tree_LDP/";

  vector<TString> ZJetsFileNames;
  ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-100to200.root");
  ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-200to400.root");
  ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-400to600.root");
  ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-600to800.root");
  ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-800to1200.root");
  ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-1200to2500.root");
  ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-2500toInf.root");
  TChain* ZJets = new TChain("tree");
  for( int i = 0 ; i < ZJetsFileNames.size() ; i++ ){
    ZJets->Add(skimType+"/"+ZJetsFileNames[i]);
  }
  samples.push_back(new RA2bTree(ZJets));
  sampleNames.push_back("ZJets");

  if( region == 0 )
    skimType="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV10/tree_GJet_CleanVars/";
  if( region == 1 )
    skimType="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV10/tree_GJetLDP_CleanVars/";

  vector<TString> GJetsFileNames;
  GJetsFileNames.push_back("tree_GJets_HT-100to200.root");
  GJetsFileNames.push_back("tree_GJets_HT-200to400.root");
  GJetsFileNames.push_back("tree_GJets_HT-400to600.root");
  GJetsFileNames.push_back("tree_GJets_HT-600toInf.root");
  //GJetsFileNames.push_back("tree_GJets_DR-0p4_HT-100to200.root");
  //GJetsFileNames.push_back("tree_GJets_DR-0p4_HT-200to400.root");
  //GJetsFileNames.push_back("tree_GJets_DR-0p4_HT-400to600.root");
  //GJetsFileNames.push_back("tree_GJets_DR-0p4_HT-600toInf.root");
  TChain* GJets = new TChain("tree");
  for( int i = 0 ; i < GJetsFileNames.size() ; i++ ){
    GJets->Add(skimType+"/"+GJetsFileNames[i]);
  }
  samples.push_back(new RA2bTree(GJets));
  sampleNames.push_back("GJets");

  for( int iSample = 0 ; iSample < samples.size() ; iSample++){

    RA2bTree* ntuple = samples[iSample];
    for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++){
      plots[iPlot].addNtuple(ntuple,sampleNames[iSample]);
      plots[iPlot].dataHist=NULL;
    }
    int numEvents = ntuple->fChain->GetEntries();
    ntupleBranchStatus<RA2bTree>(ntuple);
    for( int iEvt = 0 ; iEvt < numEvents ; iEvt++ ){
      ntuple->GetEntry(iEvt);
      if( iEvt % 1000000 == 0 ) cout << sampleNames[iSample] << ": " << iEvt << "/" << numEvents << endl;
      
      if( sampleNames[iSample] == "GJets" && !isPromptPhoton(ntuple) ) continue;
      if( !RA2bBaselineCut(ntuple) ) continue;
      for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++ ){
	plots[iPlot].fill(ntuple);
      }
    }
  }

  TFile* outputFile = new TFile("RzGamma_DR0p05_"+regionNames[region]+"_histo.root","RECREATE");

  TCanvas* can = new TCanvas("can","can",500,500);
  //can->SetTopMargin(.2);

  for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++){
    plots[iPlot].Write();
    TGraphAsymmErrors* ratio = new TGraphAsymmErrors(plots[iPlot].histoMap[samples[0]],plots[iPlot].histoMap[samples[1]],"pois");
    ratio->SetMarkerStyle(8);
    ratio->GetXaxis()->SetTitle(xlabels[iPlot]);
    ratio->GetYaxis()->SetTitle("R_{Z/#gamma}");
    ratio->GetYaxis()->SetRangeUser(0.,ratio->GetMaximum());
    ratio->Draw("Ap");
    writeExtraText = true;
    extraText = "Simulation";
    lumi_13TeV = "";
    CMS_lumi( can , 4 , 0 );
    can->Update();
    can->RedrawAxis();
    can->GetFrame()->Draw();
    
    can->SaveAs("RzGamma_DR0p05_plots/"+TString(plots[iPlot].histoMap[samples[0]]->GetName())+".png");
  }
  outputFile->Close();
}
