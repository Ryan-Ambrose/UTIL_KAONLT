#include <TProof.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

void run_peepYield(Int_t RunNumber = 0, Int_t MaxEvent = 0, Double_t threshold_cut = 0, Int_t pscal = 0)
{
  // Get RunNumber, MaxEvent, and current threshold if not provided.
  if(RunNumber == 0) {
    cout << "Enter a Run Number (-1 to exit): ";
    cin >> RunNumber;
    if( RunNumber<=0 ) return;
  }
  if(MaxEvent == 0) {
    cout << "\nNumber of Events to analyze: ";
    cin >> MaxEvent;
    if(MaxEvent == 0) {
      cerr << "...Invalid entry\n";
      exit;
    }
  }
  if(threshold_cut == 0) {
    cout << "Enter a current threshold: ";
    cin >> threshold_cut;
    if( threshold_cut<=0 ) return;
  }
  if(pscal == 0) {
    cout << "Enter a prescale factor: ";
    cin >> pscal;
    if( pscal<=0 ) return;
  }

  //Begin Scaler Efficiency Calculation
  TString rootFileNameString = Form("../../ROOTfiles/coin_replay_production_%i_%i.root",RunNumber,MaxEvent);
  TString threshold = Form("%f",threshold_cut);
  TString runNum = Form("%d",RunNumber);
  TString prescal = Form("%d", pscal);
  TString line1 = ".L coin_cut.C+";
  TString line2 = "coin_cut t(\"" + rootFileNameString + "\")";
  TString line3 = "t.Loop(\"" + runNum + "\"," + threshold + "," + prescal + ")";

  gROOT->ProcessLine(line1);
  gROOT->ProcessLine(line2);
  gROOT->ProcessLine(line3);

  //Begin Counting Good Kaon Events
  TChain ch("T");
  ch.Add(Form("../../ROOTfiles/KaonLT_coin_replay_production_%i_%i.root",RunNumber,MaxEvent));
  TString option = Form("%i",RunNumber);

  TProof *proof = TProof::Open("workers=4");
  //proof->SetProgressDialog(0);  
  ch.SetProof();
  ch.Process("peepYield.C+",option);
  proof->Close();
}
