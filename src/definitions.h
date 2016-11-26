#include "TLorentzVector.h"

// constants
// ==============================================
double bbtagCut = 0.4;
// ==============================================

double CalcdPhi( double phi1 , double phi2 ){

  double dPhi = phi1-phi2;
  if( dPhi < -TMath::Pi() ) 
    dPhi += 2*TMath::Pi() ;
  if( dPhi > TMath::Pi() )
    dPhi -= 2*TMath::Pi() ;
  return fabs(dPhi);

}


template<typename ntupleType>void ntupleBranchStatus(ntupleType* ntuple){
  ntuple->fChain->SetBranchStatus("*",0);
  ntuple->fChain->SetBranchStatus("DeltaPhi*",1);
  ntuple->fChain->SetBranchStatus("HT",1);
  ntuple->fChain->SetBranchStatus("NJets",1);
  ntuple->fChain->SetBranchStatus("BTags",1);
  ntuple->fChain->SetBranchStatus("MHT",1);
  ntuple->fChain->SetBranchStatus("Weight",1);  
  ntuple->fChain->SetBranchStatus("puWeight",1);  
  ntuple->fChain->SetBranchStatus("TriggerPass",1);  
  ntuple->fChain->SetBranchStatus("Photons*",1);  
  ntuple->fChain->SetBranchStatus("madMinPhotonDeltaR",1);  
  ntuple->fChain->SetBranchStatus("GenParticles*",1); 
  ntuple->fChain->SetBranchStatus("*Filter",1);
  ntuple->fChain->SetBranchStatus("NVtx",1);
  ntuple->fChain->SetBranchStatus("JetID",1);

}

/******************************************/
/* custom weights			   */
/******************************************/

template<typename ntupleType> double photonTriggerWeight( ntupleType* ntuple ){

  if( ntuple->Photons->size() == 0 ) return -9999.;

  double EBtrigger[4]={0.969,0.983,0.985,0.984,0.979};
  double ECtrigger[4]={0.953,0.974,0.984,0.989,0.980};

  double MHT = ntuple->MHT;
  if( ntuple->Photons_isEB->at(0) ){
    if( MHT > 250. && MHT < 300. ){
      return EBtrigger[0];
    }else if( MHT > 300. && MHT < 350. ){
      return EBtrigger[1];
    }else if( MHT > 350.  && MHT < 500. ){
      return EBtrigger[2];
    }else if( MHT > 500.  && MHT < 750. ){
      return EBtrigger[3];
    }else if( MHT > 750. ){
      return EBtrigger[4];
    }else
      return -9999.;
  }else{
    if( MHT > 250. && MHT < 300. ){
      return ECtrigger[0];
    }else if( MHT > 300. && MHT < 350. ){
      return ECtrigger[1];
    }else if( MHT > 350. && MHT < 500. ){
      return ECtrigger[2];
    }else if( MHT > 500.  && MHT < 750. ){
      return ECtrigger[3];
    }else if( MHT > 750. ){
      return ECtrigger[4];
    }else
      return -9999.;
  }
  
}

//////////////////////
//////////////////////
//////////////////////
// Photon functions //
//////////////////////
//////////////////////
//////////////////////
template<typename ntupleType> int isPromptPhoton(ntupleType* ntuple){
  return !ntuple->Photons_nonPrompt->at(0);
}

template<typename ntupleType> double photonPt(ntupleType* ntuple){
  return ntuple->Photons->at(0).Pt();
}

template<typename ntupleType> double photonEta(ntupleType* ntuple){
  return ntuple->Photons->at(0).Eta();
}

template<typename ntupleType> double photonSieie(ntupleType* ntuple){
  return ntuple->Photons_sigmaIetaIeta->at(0);
}

template<typename ntupleType> double photonIsoChrg(ntupleType* ntuple){
  return ntuple->Photons_pfChargedIsoRhoCorr->at(0);
}

template<typename ntupleType> double photonIsoGam(ntupleType* ntuple){
  return ntuple->Photons_pfGammaIsoRhoCorr->at(0);
}

template<typename ntupleType> double photonIsoNeu(ntupleType* ntuple){
  return ntuple->Photons_pfNeutralIsoRhoCorr->at(0);
}

template<typename ntupleType> double photonHoverE(ntupleType* ntuple){
  return ntuple->Photons_hadTowOverEM->at(0);
}

//////////////////////
//////////////////////
//////////////////////
// Lepton functions //
//////////////////////
//////////////////////
//////////////////////


///////////////////////////
// RECO Muon definitions //
///////////////////////////
template<typename ntupleType> int numMuons(ntupleType* ntuple){
  return ntuple->Muons->size();
}

template<typename ntupleType> double fillNumMuons(ntupleType* ntuple){
  return double(ntuple->Muons->size());
}

template<typename ntupleType> int numElectrons(ntupleType* ntuple){
  return ntuple->Electrons->size();
}

template<typename ntupleType> double fillLepPt(ntupleType* ntuple){
  if( ntuple->Electrons->size() > 0 && ntuple->Muons->size() == 0 )
    return ntuple->Electrons->at(0)->Pt();
  else if( ntuple->Electrons->size() ==0 && ntuple->Muons->size() > 0 )
    return ntuple->Muons->at(0)->Pt();
  else
    return -9999.;
}

template<typename ntupleType> double fillLepActivity(ntupleType* ntuple){
  if( ntuple->Electrons->size() > 0 && ntuple->Muons->size() == 0 )
    return ntuple->Electrons_MT2Activity->at(0);
  else if( ntuple->Electrons->size() ==0 && ntuple->Muons->size() > 0 )
    return ntuple->Muons_MT2Activity->at(0);
  else
    return -9999.;
}

//////////////////////
// END LEPTON STUFF //
//////////////////////

/**********************************************/
/* - - - - - - - PHOTON STUFF - - - - - - - - */
/**********************************************/

template<typename ntupleType> double fillPhotonChargedIso(ntupleType* ntuple){
  return 0.;
}
/**********************************************/
/* - - - - - - END PHOTON STUFF - - - - - - - */
/**********************************************/

////////////////////////////////////////////////////////////
// - - - - - - - - EVENT LEVEL VARIABLES - - - - - - - -  //
////////////////////////////////////////////////////////////
template<typename ntupleType> double fillMadMinPhotonDeltaR(ntupleType* ntuple){
  return ntuple->madMinPhotonDeltaR;
}

template<typename ntupleType> double fillPythiaMinPhotonDeltaR(ntupleType* ntuple){
  
  if( ntuple->Photons->size() == 0 ) return 999.;
  //cout << "num reco photons: " << ntuple->Photons->size() << endl;

  double minDR_genRecoPhotons = 99999.;
  int genPhotonIndex = -1;
  double dRtemp;
  for( int i = 0 ; i < ntuple->GenParticles->size() ; i++){
    if( ntuple->GenParticles_PdgId->at(i) != 22 ) continue;
    dRtemp = ntuple->GenParticles->at(i).DeltaR(ntuple->Photons->at(0));
    if( dRtemp < minDR_genRecoPhotons ){
      minDR_genRecoPhotons = dRtemp;
      genPhotonIndex = i;
    }
  }
  if( genPhotonIndex == -1 ) return 9999.;

  double minDR_partonPhoton = 999999.;

  for( int i = 0 ; i < ntuple->GenParticles->size() ; i++){
    if( abs(ntuple->GenParticles_PdgId->at(i)) > 6 && abs(ntuple->GenParticles_PdgId->at(i)) != 21 ) continue;
    if( ntuple->GenParticles->at(i).Pt() == 0 ) continue;
    //cout << "gen pdg id: " << ntuple->GenParticles_PdgId->at(i) << endl;
    //cout << "gen status: " << ntuple->GenParticles_Status->at(i) << endl;
    dRtemp = ntuple->GenParticles->at(i).DeltaR(ntuple->GenParticles->at(genPhotonIndex));
    //cout << "dRtemp: " << dRtemp << endl;
    if( dRtemp < minDR_partonPhoton ) 
      minDR_partonPhoton = dRtemp;
  }

  return minDR_partonPhoton;
}

template<typename ntupleType> double fillDeltaPhi1(ntupleType* ntuple){
  return ntuple->DeltaPhi1;
}

template<typename ntupleType> double fillDeltaPhi2(ntupleType* ntuple){
  return ntuple->DeltaPhi2;
}

template<typename ntupleType> double fillDeltaPhi3(ntupleType* ntuple){
  return ntuple->DeltaPhi3;
}

template<typename ntupleType> double fillDeltaPhi4(ntupleType* ntuple){
  return ntuple->DeltaPhi4;
}

template<typename ntupleType> double fillHT(ntupleType* ntuple){
  return ntuple->HT;
}

template<typename ntupleType> double fillMHT(ntupleType* ntuple){
  return ntuple->MHT;
}

template<typename ntupleType> double fillOne(ntupleType* ntuple){
  return 1.;
}

template<typename ntupleType> double fillNJets(ntupleType* ntuple){
  return ntuple->NJets;
}

template<typename ntupleType> double fillBTags(ntupleType* ntuple){
  return ntuple->BTags;
}

  /////////////////
 // OTHER STUFF //
/////////////////
template<typename ntupleType> double fillAnalysisBins(ntupleType* ntuple){
  double MHT = ntuple->MHT;
  double HT = ntuple->HT;

  if( MHT > 300. && MHT < 600. ){
    if( HT > 300. && HT < 1000. ){
      return 1.;
    }else if( HT > 1000. && HT < 2000. ){
      return 2.;
    }else if( HT > 2000. ){
      return 3.;
    }else 
      return -1.;
  }else if( MHT > 600. && MHT < 1000. ){
    if( HT > 600. && HT < 1000. ){
      return 4.;
    }else if( HT > 1000. && HT < 2000. ){
      return 5.;
    }else if( HT > 2000. ){
      return 6.;
    }else 
      return -1.;
  }else if( MHT > 1000. ){
    if( HT > 1000. && HT < 2000. ){
      return 7.;
    }else if( HT > 2000. ){
      return 8.;
    }else 
      return -1.;
  }else 
    return -1.;
}

template<typename ntupleType> double fillRA2b10Bins(ntupleType* ntuple){
  double MHT = ntuple->MHT;
  double HT = ntuple->HT;

  if( MHT > 300. && MHT < 350. ){
    if( HT > 300. && HT < 500. ){
      if(ntuple->NJets>=7)
	return -999999.;
      else
	return 1.;
    }else if( HT > 500. && HT < 1000. ){
      if(ntuple->NJets>=7)
	return 1.;
      else
	return 2.;
    }else if( HT > 1000. ){
      if(ntuple->NJets>=7)
	return 2.;
      else
	return 3.;
    }else 
      return -999999.;
  }else if( MHT > 350. && MHT < 500. ){
    if( HT > 350. && HT < 500. ){
      if(ntuple->NJets>=7)
	return -999999.;
      else
	return 4.;
    }else if( HT > 500. && HT < 1000. ){
      if(ntuple->NJets>=7)
	return 3.;
      else
	return 5.;
    }else if( HT > 1000. ){
      if(ntuple->NJets>=7)
	return 4.;
      else
	return 6.;
    }else 
      return -999999.;
  }else if( MHT > 500. && MHT < 750. ){
    if( HT > 500. && HT < 1000. ){
      if(ntuple->NJets>=7)
	return 5.;
      else
	return 7.;
    }else if( HT > 1000. ){
      if(ntuple->NJets>=7)
	return 6.;
      else
	return 8.;
    }else 
      return -999999.;
  }else if( MHT > 750. ){
    if( HT > 750. && HT < 1500. ){
      if(ntuple->NJets>=7)
	return 7.;
      else
	return 9.;
    }else if( HT > 1500. ){
      if(ntuple->NJets>=7)
	return 8.;
      else
	return 10.;
    }else
      return -999999.;
  }else 
    return -999999.;
}

template<typename ntupleType> double fillRA2b13Bins(ntupleType* ntuple){
  double MHT = ntuple->MHT;
  double HT = ntuple->HT;

  if( MHT > 250. && MHT < 300. ){
    if( HT > 300. && HT < 500. ) 
      if( ntuple->NJets>=7 )
	return -999999.;
      else
	return 1.;
    else if( HT > 500. && HT < 1000. )
      if( ntuple->NJets>=7 )
	return 1.;
      else
	return 2.;
    else if( HT > 1000. ) 
      if( ntuple->NJets>=7 )
	return 2.;
      else
	return 3.;
  }else if( MHT > 300. && MHT < 350. ){
    if( HT > 300. && HT < 500. ){
      if(ntuple->NJets>=7)
	return -999999.;
      else
	return 4.;
    }else if( HT > 500. && HT < 1000. ){
      if(ntuple->NJets>=7)
	return 3.;
      else
	return 5.;
    }else if( HT > 1000. ){
      if(ntuple->NJets>=7)
	return 4.;
      else
	return 6.;
    }else 
      return -999999.;
  }else if( MHT > 350. && MHT < 500. ){
    if( HT > 350. && HT < 500. ){
      if(ntuple->NJets>=7)
	return -999999.;
      else
	return 7.;
    }else if( HT > 500. && HT < 1000. ){
      if(ntuple->NJets>=7)
	return 5.;
      else
	return 8.;
    }else if( HT > 1000. ){
      if(ntuple->NJets>=7)
	return 6.;
      else
	return 9.;
    }else 
      return -999999.;
  }else if( MHT > 500. && MHT < 750. ){
    if( HT > 500. && HT < 1000. ){
      if(ntuple->NJets>=7)
	return 7.;
      else
	return 10.;
    }else if( HT > 1000. ){
      if(ntuple->NJets>=7)
	return 8.;
      else
	return 11.;
    }else 
      return -999999.;
  }else if( MHT > 750. ){
    if( HT > 750. && HT < 1500. ){
      if(ntuple->NJets>=7)
	return 9.;
      else
	return 12.;
    }else if( HT > 1500. ){
      if(ntuple->NJets>=7)
	return 10.;
      else
	return 13.;
    }else
      return -999999.;
  }else 
    return -999999.;
}

template<typename ntupleType> double fillRA2b59Bins( ntupleType* ntuple ){

  int BTags = int(ntuple->BTags);
  if( BTags != 0 ) return -999999.;
  int NJets = int(ntuple->NJets);

  if( NJets == 2 ){
      return fillRA2b13Bins(ntuple);
  }else if( NJets >= 3 && NJets <=4 ){
    return 13.+fillRA2b13Bins(ntuple);
  }else if( NJets >= 5 && NJets <= 6 ){
    return 26.+fillRA2b13Bins(ntuple);
  }else if( NJets >= 7 && NJets <= 8 ){
    return 39.+fillRA2b13Bins(ntuple);
  }else if( NJets >= 9 ){
    return 49.+fillRA2b13Bins(ntuple);
  }else 
    return -999999.;  
}

template<typename ntupleType> double fillRA2b46Bins( ntupleType* ntuple ){

  int BTags = int(ntuple->BTags);
  if( BTags != 0 ) return -999999.;
  int NJets = int(ntuple->NJets);

  if( NJets == 2 ){
    return fillRA2b10Bins(ntuple);
  }else if( NJets >= 3 && NJets <=4 ){
    return 10.+fillRA2b10Bins(ntuple);
  }else if( NJets >= 5 && NJets <= 6 ){
    return 20.+fillRA2b10Bins(ntuple);
  }else if( NJets >= 7 && NJets <= 8 ){
    return 30.+fillRA2b10Bins(ntuple);
  }else if( NJets >= 9 ){
    return 38.+fillRA2b10Bins(ntuple);
  }else 
    return -999999.;  
}

template<typename ntupleType> double fillRA2bNJet2Bins( ntupleType* ntuple ){

  int BTags = int(ntuple->BTags);
  int NJets = int(ntuple->NJets);
  
  if( NJets == 2 ){
    if( BTags == 0 ) 
      return fillRA2b10Bins(ntuple);
    else if( BTags == 1 )
      return 10.+fillRA2b10Bins(ntuple);
    else if( BTags >= 2 )
      return 20.+fillRA2b10Bins(ntuple);
  }else
    return -999999.;
}

template<typename ntupleType> double fillRA2b174Bins( ntupleType* ntuple ){

  int BTags = int(ntuple->BTags);
  int NJets = int(ntuple->NJets);
  
  if( NJets == 2 ){
    if( BTags == 0 ){
      return fillRA2b10Bins(ntuple);
    }else if( BTags == 1 ){
      return 10.+fillRA2b10Bins(ntuple);
    }else if( BTags>=2 ){
      return 20.+fillRA2b10Bins(ntuple);
    }else
      return -999999.;
  }else if( NJets >= 3 && NJets <=4 ){
    if( BTags == 0 ) 
      return 30.+fillRA2b10Bins(ntuple);
    else if( BTags == 1 )
      return 40.+fillRA2b10Bins(ntuple);
    else if( BTags == 2 )
      return 50.+fillRA2b10Bins(ntuple);
    else if( BTags >= 3 )
      return 60.+fillRA2b10Bins(ntuple);
  }else if( NJets >= 5 && NJets <= 6 ){
    if( BTags == 0 ) 
      return 70.+fillRA2b10Bins(ntuple);
    else if( BTags == 1 )
      return 80.+fillRA2b10Bins(ntuple);
    else if( BTags == 2 )
      return 90.+fillRA2b10Bins(ntuple);
    else if( BTags >= 3 )
      return 100.+fillRA2b10Bins(ntuple);
  }else if( NJets >= 7 && NJets <= 8 ){
    if( BTags == 0 )
      return 110.+fillRA2b10Bins(ntuple);
    else if( BTags == 1 )
      return 118.+fillRA2b10Bins(ntuple);
    else if( BTags == 2 )
      return 126.+fillRA2b10Bins(ntuple);
    else if( BTags >= 3 )
      return 134.+fillRA2b10Bins(ntuple);
  }else if( NJets >= 9 ){
    if( BTags == 0 ) 
      return 142.+fillRA2b10Bins(ntuple);
    else if( BTags == 1 )
      return 150.+fillRA2b10Bins(ntuple);
    else if( BTags == 2 )
      return 158.+fillRA2b10Bins(ntuple);
    else if( BTags >= 3 )
      return 166.+fillRA2b10Bins(ntuple);
  }else 
    return -999999.;  
}

template<typename ntupleType> bool ptBinCut(double pt , int ithBin){
  if( ithBin > 5 ) return false;
  double ptCut[6] = {300.,400.,500.,700.,1000.,999999.};
  return pt>ptCut[ithBin] && pt<ptCut[ithBin+1];
}
 
template<typename ntupleType> bool RA2bBaselineCut(ntupleType* ntuple){

  double DeltaPhi1 = ntuple->DeltaPhi1;
  double DeltaPhi2 = ntuple->DeltaPhi2;
  double DeltaPhi3 = ntuple->DeltaPhi3;
  double DeltaPhi4 = ntuple->DeltaPhi4;

  double HT = ntuple->HT;
  double MHT = ntuple->MHT;
  int NJets = ntuple->NJets;

  return ( (NJets==2 && DeltaPhi1>0.5 && DeltaPhi2>0.5) 
           || (NJets == 3 && DeltaPhi1 > 0.5 && DeltaPhi2 > 0.5 && DeltaPhi3 > 0.3) 
           || (NJets > 3 && DeltaPhi1 > 0.5 && DeltaPhi2 > 0.5 && DeltaPhi3 > 0.3 && DeltaPhi4 > 0.3 ) )
      && MHT>250. && HT>300. 
      && ntuple->HBHENoiseFilter==1 
      && ntuple->HBHEIsoNoiseFilter==1 
      && ntuple->eeBadScFilter==1 
      && ntuple->EcalDeadCellTriggerPrimitiveFilter == 1 
      && ntuple->NVtx>0 
      && ntuple->JetID == 1 
      ;

}

template<typename ntupleType> bool RA2bLDPBaselineCut(ntupleType* ntuple){

  double DeltaPhi1 = ntuple->DeltaPhi1;
  double DeltaPhi2 = ntuple->DeltaPhi2;
  double DeltaPhi3 = ntuple->DeltaPhi3;
  double DeltaPhi4 = ntuple->DeltaPhi4;

  double HT = ntuple->HT;
  double MHT = ntuple->MHT;
  int NJets = ntuple->NJets;

  return NJets >= 2 && MHT > 250. && HT > 300. 
      && (DeltaPhi1 < 0.5 || DeltaPhi2 < 0.5 || DeltaPhi3 < 0.3 || DeltaPhi4 < 0.3)
      && ntuple->HBHENoiseFilter==1 
      && ntuple->HBHEIsoNoiseFilter==1 
      && ntuple->eeBadScFilter==1 
      && ntuple->EcalDeadCellTriggerPrimitiveFilter == 1 
      && ntuple->NVtx>0 
      && ntuple->JetID == 1 
      ;

}

