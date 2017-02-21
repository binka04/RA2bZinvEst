// ROOT
#include "TString.h"
#include "TChain.h"
#include "TH1F.h"
#include "TROOT.h"
#include "THStack.h"
#include "TPad.h"

// STL
#include <vector>
#include <map>
#include <iostream>
#include <cassert>
#include <iomanip>

// custom
#include "plotterUtils.cc"
#include "skimSamples.cc"
#include "definitions.h"
#include "RA2bTree.cc"

using namespace std;

static const int MAX_EVENTS = 999999999;

int main(int argc, char** argv){

    gROOT->ProcessLine(".L ~/tdrstyle.C");
    gROOT->ProcessLine("setTDRStyle()");

    if(argc != 2 ) return 1;
  
    int regInt = atoi(argv[1]);
    skimSamples::region reg = static_cast<skimSamples::region>(regInt);
    skimSamples skims(reg);
    typedef plot<RA2bTree> plot;
    cout << skims.skimType << endl;
    for( int i = 0 ; i < skims.ntuples.size() ; i++){
        cout << "sample name: " << skims.sampleName[i] << endl;
    }
  
    plot Bins46plot(*fillRA2b46Bins<RA2bTree>,"AnalysisBins_BTag0_"+skims.regionNames[regInt]+"_baseline","i^{th} Bin",46,0.5,46.5);
    double NJetsBins[5]={1.5,2.5,4.5,6.5,12.5};
    plot NJetsplot(*fillNJets<RA2bTree>,"NJets_"+skims.regionNames[regInt]+"_baseline","n_{j}",4,NJetsBins);
    plot PhotonMinDeltaR_inc(*fillRecoPhotonDeltaR<RA2bTree>,"PhotonMinDeltaR_"+skims.regionNames[regInt]+"_baseline","i^{th} Bin",40,0,4);

    TString binNum;
    vector<plot> analysisBinnedPlots;
    for( int i = 0 ; i < 46 ; i++ ){
        binNum="";
        binNum+=i;
        plot PhotonMinDeltaR(*fillRecoPhotonDeltaR<RA2bTree>,"PhotonMinDeltaR_"+skims.regionNames[regInt]+"_baseline_bin"+binNum,"min#Delta R(jet,#gamma)",40,0,4);
        analysisBinnedPlots.push_back(PhotonMinDeltaR);
    }

    vector<plot> nJetsBinnedPlots;
    for( int i = 0 ; i < 4 ; i++ ){
        binNum="";
        binNum+=i;
        plot PhotonMinDeltaR(*fillRecoPhotonDeltaR<RA2bTree>,"PhotonMinDeltaR_"+skims.regionNames[regInt]+"_baseline_NJets"+binNum,"min#Delta R(jet,#gamma)",40,0,4);
        nJetsBinnedPlots.push_back(PhotonMinDeltaR);
    }

    int analysisBin = 0 ;
    int nJetBin = 0 ;

    // background MC samples
    for( int iSample = 0 ; iSample < skims.ntuples.size() ; iSample++){

        if( skims.sampleName[iSample] == "Other" || skims.sampleName[iSample] == "TT" ) continue;

        RA2bTree* ntuple = skims.ntuples[iSample];

        Bins46plot.addNtuple(ntuple,skims.sampleName[iSample]);
        Bins46plot.setFillColor(ntuple,skims.fillColor[iSample]);
        NJetsplot.addNtuple(ntuple,skims.sampleName[iSample]);
        NJetsplot.setFillColor(ntuple,skims.fillColor[iSample]);
        PhotonMinDeltaR_inc.addNtuple(ntuple,skims.sampleName[iSample]);
        PhotonMinDeltaR_inc.setFillColor(ntuple,skims.fillColor[iSample]);

        for( int iPlot = 0 ; iPlot < analysisBinnedPlots.size() ; iPlot++){
            analysisBinnedPlots[iPlot].addNtuple(ntuple,skims.sampleName[iSample]);
            analysisBinnedPlots[iPlot].setFillColor(ntuple,skims.fillColor[iSample]);
        }
        for( int iPlot = 0 ; iPlot < nJetsBinnedPlots.size() ; iPlot++){
            nJetsBinnedPlots[iPlot].addNtuple(ntuple,skims.sampleName[iSample]);
            nJetsBinnedPlots[iPlot].setFillColor(ntuple,skims.fillColor[iSample]);
        }

        int numEvents = ntuple->fChain->GetEntries();
        ntupleBranchStatus<RA2bTree>(ntuple);
        double weight = 1.0;
        for( int iEvt = 0 ; iEvt < min(MAX_EVENTS,numEvents) ; iEvt++ ){
            ntuple->GetEntry(iEvt);
            if( iEvt % 1000000 == 0 ) cout << skims.sampleName[iSample] << ": " << iEvt << "/" << numEvents << endl;

            if( ( reg == skimSamples::kSignal || reg == skimSamples::kPhoton || reg == skimSamples::kDYe || reg == skimSamples::kDYm ) && !RA2bBaselineCut(ntuple) ) continue;
            if( ( reg == skimSamples::kLDP || reg == skimSamples::kPhotonLDP || reg == skimSamples::kDYeLDP || reg == skimSamples::kDYmLDP ) && !RA2bLDPBaselineCut(ntuple) ) continue;

            if( skims.regionNames[regInt] == "photonLDP" || skims.regionNames[regInt] == "photon" ){
                if( skims.sampleName[iSample] == "QCD" && ntuple->madMinPhotonDeltaR > 0.4 && isPromptPhoton(ntuple) ) continue;
                if( skims.sampleName[iSample] == "GJets" && ( !isPromptPhoton(ntuple) || ntuple->madMinPhotonDeltaR<0.4 ) ) continue;
                if( ntuple->Photons->size() != 1 ) continue;
                if( ntuple->Photons->at(0).Pt() < 200. ) continue;
            }

            weight = lumi*ntuple->Weight*ntuple->puWeight;
            if( reg == skimSamples::kPhoton || reg == skimSamples::kPhotonLDP )
                weight*=photonTriggerWeight(ntuple);
            if( skims.sampleName[iSample] == "GJets" ) 
                weight*=GJets0p4Weights(ntuple);

            analysisBin = Bins46plot.fill(ntuple,lumi*ntuple->Weight*ntuple->puWeight);
            nJetBin = NJetsplot.fill(ntuple,lumi*ntuple->Weight*ntuple->puWeight);
            PhotonMinDeltaR_inc.fill(ntuple,lumi*ntuple->Weight*ntuple->puWeight);
            

            if( analysisBin > 0 && analysisBin <= 46 ){
                    analysisBinnedPlots[analysisBin-1].fill(ntuple);
            }

            if( nJetBin > 0 && nJetBin <= 4 ){
                    nJetsBinnedPlots[nJetBin-1].fill(ntuple);
            }
            

        }
    }

    // Data samples
    RA2bTree* ntuple = skims.dataNtuple;
    Bins46plot.addDataNtuple(ntuple,"data");
    NJetsplot.addDataNtuple(ntuple,"data");
    PhotonMinDeltaR_inc.addDataNtuple(ntuple,"data");
    for( int iPlot = 0 ; iPlot < analysisBinnedPlots.size() ; iPlot++){
        analysisBinnedPlots[iPlot].addDataNtuple(ntuple,"data");
    }
    for( int iPlot = 0 ; iPlot < nJetsBinnedPlots.size() ; iPlot++){
        nJetsBinnedPlots[iPlot].addDataNtuple(ntuple,"data");
    }
    int numEvents = ntuple->fChain->GetEntries();
    ntupleBranchStatus<RA2bTree>(ntuple);
    for( int iEvt = 0 ; iEvt < min(MAX_EVENTS,numEvents) ; iEvt++ ){
        ntuple->GetEntry(iEvt);
        if( iEvt % 1000000 == 0 ) cout << "data: " << iEvt << "/" << numEvents << endl;

        if( ( reg == skimSamples::kSignal || reg == skimSamples::kPhoton || reg == skimSamples::kDYe || reg == skimSamples::kDYm ) && !RA2bBaselineCut(ntuple) ) continue;
        if( ( reg == skimSamples::kLDP || reg == skimSamples::kPhotonLDP || reg == skimSamples::kDYeLDP || reg == skimSamples::kDYmLDP ) && !RA2bLDPBaselineCut(ntuple) ) continue;    
        if( ( reg == skimSamples::kPhoton || reg == skimSamples::kPhotonLDP ) && ntuple->Photons->at(0).Pt()<200. ) continue;

        if( ntuple->TriggerPass->size() < 44 || !ntuple->TriggerPass->at(51) ) continue;

        analysisBin = Bins46plot.fillData(ntuple);        
        nJetBin = NJetsplot.fillData(ntuple);        
        PhotonMinDeltaR_inc.fillData(ntuple);
        if( analysisBin > 0 && analysisBin <= 46 ){
                analysisBinnedPlots[analysisBin-1].fillData(ntuple);
        }
        if( nJetBin > 0 && nJetBin <= 4 ){
                nJetsBinnedPlots[nJetBin-1].fillData(ntuple);
        }
    }

    cout << "DONE FILLING HISTOGRAMS" << endl;

    TFile* outputFile = new TFile("fragmentationFits.root","UPDATE");

    TCanvas* can = new TCanvas("can","can",500,500);
    can->SetTopMargin(0.05);
    Bins46plot.Write();
    Bins46plot.Draw(can,skims.ntuples,skims.signalNtuples,"../plots/fragmentationFits/");

    PhotonMinDeltaR_inc.Write();
    PhotonMinDeltaR_inc.Draw(can,skims.ntuples,skims.signalNtuples,"../plots/fragmentationFits/");
    delete can;

    for( int iPlot = 0 ; iPlot < analysisBinnedPlots.size() ; iPlot++){
        TCanvas* can = new TCanvas("can","can",500,500);
        can->SetTopMargin(0.05);
        analysisBinnedPlots[iPlot].Write();
        analysisBinnedPlots[iPlot].Draw(can,skims.ntuples,skims.signalNtuples,"../plots/fragmentationFits/");
    }

    for( int iPlot = 0 ; iPlot < nJetsBinnedPlots.size() ; iPlot++){
        TCanvas* can = new TCanvas("can","can",500,500);
        can->SetTopMargin(0.05);
        nJetsBinnedPlots[iPlot].Write();
        nJetsBinnedPlots[iPlot].Draw(can,skims.ntuples,skims.signalNtuples,"../plots/fragmentationFits/");
    }

    cout << "bin & MC & data & data/MC & GJets & data-QCD & frag. frac. \\\\ \\hline" << endl;
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    int QCDindex=-1,GJetsIndex=-1;
    auto it = std::find(skims.sampleName.begin(), skims.sampleName.end(), TString("QCD"));
    if (it == skims.sampleName.end()){
        // name not in vector
    }else{
        QCDindex = std::distance(skims.sampleName.begin(), it);
    }
    it = std::find(skims.sampleName.begin(), skims.sampleName.end(), TString("GJets"));
    if (it == skims.sampleName.end()){
        // name not in vector
    }else{
        GJetsIndex = std::distance(skims.sampleName.begin(), it);
    }
    if( QCDindex < 0 || GJetsIndex < 0 ) 
        assert(0) ;

    vector<plot> myPlots = nJetsBinnedPlots;
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    for( int iPlot = 0 ; iPlot < myPlots.size() ; iPlot++){
        double MCerror=0,QCDerror=0;
        for( int iBin = 1 ; iBin <= 4 ; iBin++ ){
            if( myPlots[iPlot].sum->GetBinContent(iBin) != 0 )
                MCerror+=myPlots[iPlot].sum->GetBinError(iBin)*myPlots[iPlot].sum->GetBinError(iBin);
            if( myPlots[iPlot].histoMap[skims.ntuples[QCDindex]]->GetBinContent(iBin) != 0 )
                QCDerror+=myPlots[iPlot].histoMap[skims.ntuples[QCDindex]]->GetBinError(iBin)*myPlots[iPlot].histoMap[skims.ntuples[QCDindex]]->GetBinError(iBin);
        }
        MCerror = sqrt(MCerror);
        QCDerror = sqrt(QCDerror);
        double dataError=sqrt(myPlots[iPlot].dataHist->Integral(1,4));
        double lowMC = myPlots[iPlot].sum->Integral(1,4);
        double lowData = myPlots[iPlot].dataHist->Integral(1,4);
        double highGJets = myPlots[iPlot].histoMap[skims.ntuples[GJetsIndex]]->Integral(4,40);
        double highQCD = myPlots[iPlot].histoMap[skims.ntuples[QCDindex]]->Integral(4,40);
        double highData = myPlots[iPlot].dataHist->Integral(4,40);

        cout << iPlot << " & " << lowMC << " $\\pm$ " << MCerror ;
        cout << std::setprecision(5) << " & " << lowData << " $\\pm$ " << std::setprecision(2) << dataError ; 
        cout << std::setprecision(2) << " & " << lowData/lowMC << " $\\pm$ " << std::setprecision(2) << sqrt(MCerror*MCerror/lowMC/lowMC+dataError*dataError/lowData/lowData)*lowData/lowMC ;
        cout << std::setprecision(5) << " & " << highGJets << " $\\pm$ " << std::setprecision(2) << sqrt(highGJets) ;
        cout << std::setprecision(5) << " & " << highData-highQCD << " $\\pm$ " << std::setprecision(2) << sqrt(highData+QCDerror*QCDerror) ;
        cout << std::setprecision(2) << " & " << (highData-highQCD)/(lowData+highData-highQCD) << " $\\pm$ " << std::setprecision(2) << sqrt(1./highData+1./lowData+QCDerror*QCDerror/highQCD/highQCD)*(highData-highQCD)/(lowData+highData-highQCD) ; 
        cout << " \\\\ \\hline " <<  endl;        
    
    }
    outputFile->Close();

}
