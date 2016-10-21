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
#include <fstream>

#include "CMS_lumi.cc"

#include "plotterUtils.cc"
#include "skimSamples.cc"
#include "definitions.h"
#include "RA2bTree.cc"

using namespace std;

/////////////////////////////////////////////////////////////////
//  - - - - - - - - Triggers for V10 ntuples - - - - - - - - - //
// 0 HLT_CaloJet500_NoJetID_v2				       //
// 1 HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v4	       //
// 2 HLT_DoubleMu8_Mass8_PFHT300_v4			       //
// 3 HLT_Ele105_CaloIdVT_GsfTrkIdT_v3			       //
// 4 HLT_Ele115_CaloIdVT_GsfTrkIdT_v2			       //
// 5 HLT_Ele15_IsoVVVL_PFHT350_PFMET50_v2		       //
// 6 HLT_Ele15_IsoVVVL_PFHT350_v2			       //
// 7 HLT_Ele15_IsoVVVL_PFHT400_v			       //
// 8 HLT_Ele15_IsoVVVL_PFHT600_v3			       //
// 9 HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v3	       //
// 10 HLT_Ele25_eta2p1_WPTight_Gsf_v 			       //
// 11 HLT_Ele27_WPTight_Gsf_v1				       //
// 12 HLT_Ele27_eta2p1_WPLoose_Gsf_v 			       //
// 13 HLT_Ele45_WPLoose_Gsf_v1				       //
// 14 HLT_Ele50_IsoVVVL_PFHT400_v			       //
// 15 HLT_IsoMu16_eta2p1_MET30_v1			       //
// 16 HLT_IsoMu22_eta2p1_v				       //
// 17 HLT_IsoMu22_v2					       //
// 18 HLT_IsoMu24_v 					       //
// 19 HLT_IsoTkMu22_v2					       //
// 20 HLT_Mu15_IsoVVVL_PFHT350_PFMET50_v2		       //
// 21 HLT_Mu15_IsoVVVL_PFHT350_v2			       //
// 22 HLT_Mu15_IsoVVVL_PFHT400_v			       //
// 23 HLT_Mu15_IsoVVVL_PFHT600_v3			       //
// 24 HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v2		       //
// 25 HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v2		       //
// 26 HLT_Mu45_eta2p1_v2				       //
// 27 HLT_Mu50_IsoVVVL_PFHT400_v			       //
// 28 HLT_Mu50_v2					       //
// 29 HLT_PFHT200_v2					       //
// 30 HLT_PFHT250_v2					       //
// 31 HLT_PFHT300_PFMET100_v1				       //
// 32 HLT_PFHT300_PFMET110_v1				       //
// 33 HLT_PFHT300_v2					       //
// 34 HLT_PFHT350_v3					       //
// 35 HLT_PFHT400_v2					       //
// 36 HLT_PFHT475_v2					       //
// 37 HLT_PFHT600_v3					       //
// 38 HLT_PFHT650_v3					       //
// 39 HLT_PFHT800_v2					       //
// 40 HLT_PFHT900_v1					       //
// 41 HLT_PFMET100_PFMHT100_IDTight_v2			       //
// 42 HLT_PFMET110_PFMHT110_IDTight_v2			       //
// 43 HLT_PFMET120_PFMHT120_IDTight_v2			       //
// 44 HLT_PFMET90_PFMHT90_IDTight_v2			       //
// 45 HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_v1		       //
// 46 HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v1		       //
// 47 HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v1		       //
// 48 HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_v1		       //
// 49 HLT_Photon165_HE10_v3				       //
// 50 HLT_Photon175_v3					       //
// 51 HLT_Photon90_CaloIdL_PFHT500_v3			       //
// 52 HLT_TkMu50_v					       //
/////////////////////////////////////////////////////////////////

int main(int argc, char** argv){

  if( argc < 3 ){
    cout << "need two arguments" << endl;
    cout << "Arg. 1 - first file index" << endl;
    cout << "Arg. 2 - last file index" << endl;
  }

  gROOT->ProcessLine(".L ~/tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle()");

  typedef plot<RA2bTree> plot;
  
  lumi = 24500;

  vector<TString> xlabels;
  xlabels.push_back("MHT [GeV]");
  xlabels.push_back("H_{T} [GeV]");
  xlabels.push_back("n_{j}");

  plot tarMHTplot(*fillMHT<RA2bTree>,"MHT_tarTrigger","H_{T}^{miss} [GeV]",20,100.,2100.);
  plot tarHTplot(*fillHT<RA2bTree>,"HT_tarTrigger","H_{T} [GeV]",30,100,3100.);
  plot tarNJetsplot(*fillNJets<RA2bTree>,"NJets_tarTrigger","n_{j}",14,1.5,15.5);

  plot refMHTplot(*fillMHT<RA2bTree>,"MHT_refTrigger","H_{T}^{miss} [GeV]",20,100.,2100.);
  plot refHTplot(*fillHT<RA2bTree>,"HT_refTrigger","H_{T} [GeV]",30,100,3100.);
  plot refNJetsplot(*fillNJets<RA2bTree>,"NJets_refTrigger","n_{j}",14,1.5,15.5);

  vector<plot> plotsTarget;
  plotsTarget.push_back(tarMHTplot);
  plotsTarget.push_back(tarHTplot);
  plotsTarget.push_back(tarNJetsplot);
  vector<plot> plotsReference;
  plotsReference.push_back(refMHTplot);
  plotsReference.push_back(refHTplot);
  plotsReference.push_back(refNJetsplot);

  // background MC samples
  vector<RA2bTree*> samples;
  vector<TString> sampleNames;

  TChain* JetHTdata = new TChain("TreeMaker2/PreSelection");

  ifstream in_stream;
  string line;
  in_stream.open("JetHT_inputs.txt");
  vector<TString> fileNames;
  while(!in_stream.eof()){
    in_stream >> line;
    if( line.find("Run2016B") == -1 ) continue;
    fileNames.push_back(TString(line));
  }
  in_stream.close();

  for( int i = atoi(argv[1]) ; i < atoi(argv[2]) ; i++ ){
    JetHTdata->Add(fileNames[i]);
  }

  samples.push_back(new RA2bTree(JetHTdata));
  sampleNames.push_back("JetHTdata");

  for( int iSample = 0 ; iSample < samples.size() ; iSample++){

    RA2bTree* ntuple = samples[iSample];
    for( int iPlot = 0 ; iPlot < plotsTarget.size() ; iPlot++){
      plotsTarget[iPlot].addDataNtuple(ntuple,sampleNames[iSample]);
      plotsReference[iPlot].addDataNtuple(ntuple,sampleNames[iSample]);
    }

    int numEvents = ntuple->fChain->GetEntries();
    ntupleBranchStatus<RA2bTree>(ntuple);
    for( int iEvt = 0 ; iEvt < numEvents ; iEvt++ ){
      ntuple->GetEntry(iEvt);
      if( iEvt % 1000000 == 0 ) cout << sampleNames[iSample] << ": " << iEvt << "/" << numEvents << endl;
      if( ntuple->TriggerPass->size() > 40 && ntuple->TriggerPass->at(40) == 1 && ntuple->Photons->size() > 0 && ntuple->Photons->at(0).Pt()>100. ) continue;
      for( int iPlot = 0 ; iPlot < plotsTarget.size() ; iPlot++ ){
	plotsReference[iPlot].fillData(ntuple);
	if( ntuple->TriggerPass->size() > 50 && ntuple->TriggerPass->at(50) == 1 ){
	  plotsTarget[iPlot].fillData(ntuple);
	}
      }
    }
  }

  TFile* outputFile = new TFile("triggerEff_"+TString(argv[1])+"_"+TString(argv[2])+"_histo.root","RECREATE");

  TCanvas* can = new TCanvas("can","can",500,500);

  for( int iPlot = 0 ; iPlot < plotsTarget.size() ; iPlot++){
    plotsTarget[iPlot].Write();
    plotsReference[iPlot].Write();
    TGraphAsymmErrors* ratio = new TGraphAsymmErrors(plotsTarget[iPlot].dataHist,plotsReference[iPlot].dataHist);
    ratio->SetMarkerStyle(8);
    ratio->GetXaxis()->SetTitle(xlabels[iPlot]);
    ratio->GetYaxis()->SetTitle("#epsilon");
    ratio->GetYaxis()->SetRangeUser(0.,ratio->GetMaximum());
    ratio->Draw("Ap");
    writeExtraText = true;
    extraText = "Simulation";
    lumi_13TeV = "";
    CMS_lumi( can , 4 , 0 );
    can->Update();
    can->RedrawAxis();
    can->GetFrame()->Draw();
    
    can->SaveAs("triggerEff_plots/"+TString(plotsTarget[iPlot].dataHist->GetName())+".png");
  }
  outputFile->Close();
}
