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

    TFile *f1 = new TFile("L1PrefiringMaps_new.root");
    TH2F* h_photon = (TH2F*)f1->Get("L1prefiring_photonptvseta_2016BtoH");
    TH2F* h_jet = (TH2F*)f1->Get("L1prefiring_jetptvseta_2016BtoH");
    int c = 0, d = 0;
    TAxis* xAxis_photon = h_photon->GetXaxis();
    TAxis* yAxis_photon = h_photon->GetYaxis();
    double binXLow_photon[72] ,binXUp_photon[72] ,binYLow_photon[72] ,binYUp_photon[72] ,binCont_photon[72], binContErr_photon[72];
    for(int i = 1; i<= h_photon->GetNbinsY(); i++ ){
                for(int j = 1; j<= h_photon->GetNbinsX(); j++ ) {
                        if(h_photon->GetBinContent(j,i) != 0){
          	                 binXLow_photon[c] = xAxis_photon->GetBinLowEdge(j);
                	         binXUp_photon[c] = xAxis_photon->GetBinUpEdge(j);
                       	         binYLow_photon[c] = yAxis_photon->GetBinLowEdge(i);
                   		 binYUp_photon[c] = yAxis_photon->GetBinUpEdge(i);
                                 binCont_photon[c] = h_photon->GetBinContent(j,i);
                                 binContErr_photon[c] = h_photon->GetBinError(j,i);
                                 c++;
                        }
                }
     }

    TAxis* xAxis_jet = h_jet->GetXaxis();
    TAxis* yAxis_jet = h_jet->GetYaxis();
    double binXLow_jet[89] ,binXUp_jet[89] ,binYLow_jet[89] ,binYUp_jet[89] ,binCont_jet[89], binContErr_jet[89];
    for(int i = 1; i<= h_jet->GetNbinsY(); i++ ){
                for(int j = 1; j<= h_jet->GetNbinsX(); j++ ) {
                        if(h_jet->GetBinContent(j,i) != 0){
          	                 binXLow_jet[d] = xAxis_jet->GetBinLowEdge(j);
                	         binXUp_jet[d] = xAxis_jet->GetBinUpEdge(j);
                       	         binYLow_jet[d] = yAxis_jet->GetBinLowEdge(i);
                   		 binYUp_jet[d] = yAxis_jet->GetBinUpEdge(i);
                                 binCont_jet[d] = h_jet->GetBinContent(j,i);
                                 binContErr_jet[d] = h_jet->GetBinError(j,i);
                                 d++;
                        }
                }
     }


    bool DR0p4 = true;

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

    gROOT->ProcessLine(".L tdrstyle.C");
    gROOT->ProcessLine("setTDRStyle()");

    typedef plot<RA2bTree> plot;
  
    vector<TString> xlabels;
    xlabels.push_back("MHT [GeV]");
    xlabels.push_back("H_{T} [GeV]");
    xlabels.push_back("n_{j}");
    xlabels.push_back("n_{b}");
    xlabels.push_back("i^{th} Bin");
    xlabels.push_back("i^{th} Bin");
    xlabels.push_back("i^{th} Bin");
    xlabels.push_back("i^{th} Bin");
    xlabels.push_back("#Delta#Phi_{1}");
    xlabels.push_back("#Delta#Phi_{2}");
    xlabels.push_back("#Delta#Phi_{3}");
    xlabels.push_back("#Delta#Phi_{4}");

    plot MHTplot(*fillMHT<RA2bTree>,"MHT_RzGamma_"+regionNames[region],"H_{T}^{miss} [GeV]",15,300.,1800.);
    plot HTplot(*fillHT<RA2bTree>,"HT_RzGamma_"+regionNames[region],"H_{T} [GeV]",15,300,2800.);
    plot NJetsplot(*fillNJets<RA2bTree>,"NJets_RzGamma_"+regionNames[region],"n_{j}",14,1.5,15.5);
    plot BTagsplot(*fillBTags<RA2bTree>,"BTags_RzGamma_"+regionNames[region],"n_{b}",6,-0.5,5.5);

    plot Binsplot(*fillRA2b174Bins<RA2bTree>,"AnalysisBins_RzGamma_"+regionNames[region],"i^{th} Bin",174,0.5,174.5);
    plot Bins46plot(*fillRA2b46Bins<RA2bTree>,"AnalysisBins_BTag0_RzGamma_"+regionNames[region],"i^{th} Bin",46,0.5,46.5);
    plot Bins59plot(*fillRA2b59Bins<RA2bTree>,"AnalysisBins_BTag0plusQCDCR_RzGamma_"+regionNames[region],"i^{th} Bin",59,0.5,59.5);
    plot BinsNJet2plot(*fillRA2bNJet2Bins<RA2bTree>,"AnalysisBins_NJet2_RzGamma_"+regionNames[region],"i^{th} Bin",30,0.5,30.5);
  
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
    plots.push_back(Bins46plot);
    plots.push_back(Bins59plot);
    plots.push_back(BinsNJet2plot);
    plots.push_back(DeltaPhi1plot);
    plots.push_back(DeltaPhi2plot);
    plots.push_back(DeltaPhi3plot);
    plots.push_back(DeltaPhi4plot);
// background MC samples
   vector<RA2bTree*> samples;
    vector<TString> sampleNames;

    TString skimType;
    if( region == 0 )
        skimType=BASE_DIR+"tree_signal/";
    if( region == 1 )
        skimType=BASE_DIR+"tree_LDP/";

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
        skimType=BASE_DIR+"tree_GJet_CleanVars/";
    if( region == 1 )
        skimType=BASE_DIR+"tree_GJetLDP_CleanVars/";

    vector<TString> GJetsFileNames;
    if( DR0p4 ){ 
        GJetsFileNames.push_back("tree_GJets_DR-0p4_HT-100to200.root");
        GJetsFileNames.push_back("tree_GJets_DR-0p4_HT-200to400.root");
        GJetsFileNames.push_back("tree_GJets_DR-0p4_HT-400to600.root");
        GJetsFileNames.push_back("tree_GJets_DR-0p4_HT-600toInf.root");
    }else{
        GJetsFileNames.push_back("tree_GJets_HT-100to200.root");
        GJetsFileNames.push_back("tree_GJets_HT-200to400.root");
        GJetsFileNames.push_back("tree_GJets_HT-400to600.root");
        GJetsFileNames.push_back("tree_GJets_HT-600toInf.root");
    }
    TChain* GJets = new TChain("tree");
    for( int i = 0 ; i < GJetsFileNames.size() ; i++ ){
        GJets->Add(skimType+"/"+GJetsFileNames[i]);
    }
    samples.push_back(new RA2bTree(GJets));
    sampleNames.push_back("GJets");

    int count = 0, count1 = 0, count2 = 0, count_jet = 0, count4 = 0, count5 = 0;
   
    for( int iSample = 0 ; iSample < samples.size() ; iSample++){
        RA2bTree* ntuple = samples[iSample];
        for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++){
            plots[iPlot].addNtuple(ntuple,sampleNames[iSample]);
            plots[iPlot].dataHist=NULL;
        }
        int numEvents = ntuple->fChain->GetEntries();
        ntupleBranchStatus<RA2bTree>(ntuple);
        double weight = 1.0;
        for( int iEvt = 0 ; iEvt < numEvents ; iEvt++ ){
            ntuple->GetEntry(iEvt);
            if( iEvt % 1000000 == 0 ) cout << sampleNames[iSample] << ": " << iEvt << "/" << numEvents << endl;
            if( sampleNames[iSample] == "GJets" && ntuple->Photons->size() != 1 ) continue;      
            if( sampleNames[iSample] == "GJets" && !isPromptPhoton(ntuple) ) continue;
            if( sampleNames[iSample] == "GJets" && ntuple->Photons_fullID->at(0)!=1 ) continue;
            if( sampleNames[iSample] == "GJets" && !( ntuple->madMinPhotonDeltaR>0.4 ) ) continue;
            if( sampleNames[iSample] == "GJets" && ntuple->Photons->at(0).Pt() < 200. ) continue;      
            if( ( region == 0 && !RA2bBaselineCut(ntuple) ) || ( region == 1 && !RA2bLDPBaselineCut(ntuple) ) ) continue;
         //   if( sampleNames[iSample] == "GJets" && fabs(ntuple->Photons->at(0).Eta())>=2)continue;     
         //   if( sampleNames[iSample] == "GJets" && ((fabs(ntuple->Photons->at(0).Eta())>=1.4442 && fabs(ntuple->Photons->at(0).Eta()<=1.566))))continue;
            double prefiring_weight_photon = 1.0;
 
             if( sampleNames[iSample] == "ZJets" || sampleNames[iSample] == "GJets" ) {   count_jet++;          }

/*************************************************************** prefiring weight Photon *************************************************************************/
            if( sampleNames[iSample] == "GJets" ){  
             count++;            
    	        for(int x = 0; x < 72; x++){
       		      if(binXLow_photon[x] <= (ntuple->Photons->at(0).Eta()) && (ntuple->Photons->at(0).Eta()) < binXUp_photon[x] &&  binYLow_photon[x] <= (ntuple->Photons->at(0).Pt()) && (ntuple->Photons->at(0).Pt()) < binYUp_photon[x])
             		      prefiring_weight_photon = (1 - binCont_photon[x]);
                }
            }




/******************************************************prefiring weight Jet**************************************************************************************/


             double prefiring_weight_jet[ntuple->Jets->size()] ;
             for (int unsigned s = 0; s < ntuple->Jets->size();s++){
	        prefiring_weight_jet[s] = 1 ;
    	        for(int x = 0; x < 89; x++){

       		      if(binXLow_jet[x] <= (ntuple->Jets->at(s).Eta()) && (ntuple->Jets->at(s).Eta()) < binXUp_jet[x] &&  binYLow_jet[x] <= (ntuple->Jets->at(s).Pt()) && (ntuple->Jets->at(s).Pt()) < binYUp_jet[x])
             		      prefiring_weight_jet[s] = (1 - binCont_jet[x]) ;
                }
            }


         if(sampleNames[iSample] == "GJets" && prefiring_weight_photon != 1 ) count1++;
         if( prefiring_weight_jet[0] != 1 ) count2++;
         if(sampleNames[iSample] == "GJets" && prefiring_weight_jet[0] != 1 ) count4++;
         if(sampleNames[iSample] == "ZJets" && prefiring_weight_jet[0] != 1 ) count5++;

            weight = lumi*ntuple->Weight*customPUweights(ntuple);
            
            for (int unsigned s = 0; s < ntuple->Jets->size();s++){
            	weight*=prefiring_weight_jet[s];
            }
  

            if( sampleNames[iSample] == "GJets" && DR0p4 ) 
                weight*=GJets0p4Weights(ntuple)*dRweights(ntuple)*prefiring_weight_photon;
         
            for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++ ){
                if( sampleNames[iSample] == "GJets"){
                    plots[iPlot].fill(ntuple,weight);
		}else 
                    plots[iPlot].fill(ntuple,weight);
            }
        }       
    }
    cout<< " \tTotal GJets events\t "<<count<<"\tprefiring weighted != 1  events\t"<<count1;
    cout<< " \tTotal Jets events\t "<<count_jet<<"\tprefiring weighted != 1  events\t"<<count2<<"from GJets\t"<<count4<<"\tfrom ZJets\t"<<count5<<"\t";
    
    TFile* outputFile;
    if( DR0p4 ) 
        outputFile = new TFile("RzGamma_PUweightOnly_"+regionNames[region]+"_hist0_prefire_in_photon&Jets.root","RECREATE");
    else 
        outputFile = new TFile("RzGamma_DR0p05_PUweightOnly_"+regionNames[region]+"_histo.root","RECREATE");

    TCanvas* can = new TCanvas("can","can",500,500);

    for( int iPlot = 0 ; iPlot < plots.size() ; iPlot++){
        plots[iPlot].Write();
        TGraphAsymmErrors* ratio = new TGraphAsymmErrors(plots[iPlot].histoMap[samples[0]],plots[iPlot].histoMap[samples[1]],"pois");
        ratio->SetMarkerStyle(8);
        ratio->GetXaxis()->SetTitle(xlabels[iPlot]);
        ratio->GetYaxis()->SetTitle("R_{Z/#gamma}");
        ratio->GetYaxis()->SetRangeUser(0.,2.);
        ratio->Draw("Ap");
        writeExtraText = true;
        extraText = "Simulation";
        lumi_13TeV = "";
        CMS_lumi( can , 4 , 0 );
        can->Update();
        can->RedrawAxis();
        can->GetFrame()->Draw();
    
        if( DR0p4 ){
            can->SaveAs("../plots/RzGamma_plots/"+TString(plots[iPlot].histoMap[samples[0]]->GetName())+".png");
            can->SaveAs("../plots/RzGamma_plots/"+TString(plots[iPlot].histoMap[samples[0]]->GetName())+".pdf");
            can->SaveAs("../plots/RzGamma_plots/"+TString(plots[iPlot].histoMap[samples[0]]->GetName())+".eps");
        }else{
            can->SaveAs("../plots/RzGamma_DR0p05_plots/"+TString(plots[iPlot].histoMap[samples[0]]->GetName())+".png");
            can->SaveAs("../plots/RzGamma_DR0p05_plots/"+TString(plots[iPlot].histoMap[samples[0]]->GetName())+".pdf");
            can->SaveAs("../plots/RzGamma_DR0p05_plots/"+TString(plots[iPlot].histoMap[samples[0]]->GetName())+".eps");
        }
    }

    outputFile->Close();
}
    
