#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TChain.h"
#include "TH1F.h"
#include "TROOT.h"
#include "THStack.h"
#include "TPad.h"

#include <algorithm>
#include <vector>
#include <map>
#include <iostream>

#include "CMS_lumi.cc"

#include "plotterUtils.cc"
//#include "skimSamples.cc"
//#include "definitions.h"
#include "bibhuDefinitions.h"
#include "BibhuTree.cc"
#include "bibhuSamples.cc"

using namespace std;

int main(int argc, char** argv){

  if( argc < 4 ){ 
    cout << "at least 3 argument needed:" << endl;
    cout << "ARGUMENT 1 - SAME NAME SHORTHAND: " << endl;
    cout << "will be used to name histograms" << endl;
    cout << "ARGUMENT 2 - first file index" << endl;
    cout << "ARGUMENT 3 - last file index" << endl;
    return 1;
  }

  typedef plot<BibhuTree> plot;
  
  lumi = 24500;

  plot MinPhotonDeltaRplot(*fillMinPhotonDeltaR<BibhuTree>,"MinPhotonDeltaR","#DeltaR_{#gamma,par}",100,0.,5.);

  vector<plot> plots;
  plots.push_back(MinPhotonDeltaRplot);

  // background MC samples
  vector<BibhuTree*> samples;
  vector<TString> sampleNames;

  /*
  TChain* tree = new TChain("TreeMaker2/PreSelection");
  for( int iFile = 3 ; iFile<argc ; iFile++){
    tree->Add(argv[iFile]);
  }
  sampleNames.push_back(argv[1]);
  samples.push_back(new BibhuTree(tree));
  */
  // initialize TChain (sample enum)
  init(atoi(argv[2]));
  TChain* tree = new TChain("TreeMaker2/PreSelection");
  for(int i = min(atoi(argv[3]),int(fileNames.size())) ; i < min(atoi(argv[4]),int(fileNames.size())) ; i++){
    tree->Add(fileNames[i]);
  }
  samples.push_back(new BibhuTree(tree));

  if( atoi(argv[2]) == 0 ) 
    sampleNames.push_back("GJets");
  if( atoi(argv[2]) == 1 ) 
    sampleNames.push_back("QCD");


  for( int iSample = 0 ; iSample < samples.size() ; iSample++){

    BibhuTree* ntuple = samples[iSample];
    for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++){
      plots[iPlot].addNtuple(ntuple,sampleNames[iSample]);
      plots[iPlot].dataHist=NULL;
    }
    int numEvents = ntuple->fChain->GetEntries();
    ntupleBranchStatus<BibhuTree>(ntuple);
    for( int iEvt = 0 ; iEvt < numEvents ; iEvt++ ){
      ntuple->GetEntry(iEvt);
      if( iEvt % 100000 == 0 ) cout << sampleNames[iSample] << ": " << iEvt << "/" << numEvents << endl;

      if( ntuple->Photons->size() == 0 ) continue; 
      if( !isPromptPhoton(ntuple) ) continue;
      if( ntuple->NJetsclean<2 || ntuple->MHTclean<200 || ntuple->HTclean<500 ) continue;
      //if( !RA2bBaselineCut(ntuple) ) continue;
      //cout << "minPhotonDeltaR: " << fillMinPhotonDeltaR(ntuple) << endl;
      for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++ ){
	plots[iPlot].fill(ntuple);
      }
    }
  }

  TFile* outputFile = new TFile("MinPhotonDR_"+TString(argv[1])+"_"+TString(argv[3])+"_"+TString(argv[4])+"_histo.root","RECREATE");

  TCanvas* can = new TCanvas("can","can",500,500);

  for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++){
    plots[iPlot].Write();
  }

  outputFile->Close();
}
