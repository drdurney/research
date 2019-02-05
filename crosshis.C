#include <iostream>
#include <math.h>
#include "TFile.h"
#include "TTree.h"


void crosshis(){
	TFile *f = TFile::Open("T0RecoSCE_ana.root");
        if(f==0){
                printf("Error: cannot open file\n");
                return;
                }
        TH3F* his = new TH3F("hPos", "Number of Events", 34, -380, 380, 30, 0, 600, 35, 0, 700);
	TTreeReader myReader("t0ana/_tree", f);
        //f->Print(); //use this to look at data types
        TTreeReaderArray<double> X_star(myReader, "_rc_xs_corr.rc_xs");
        TTreeReaderArray<double> Y_star(myReader, "_rc_ys.rc_ys");
        TTreeReaderArray<double> Z_star(myReader, "_rc_zs.rc_zs");
        TTreeReaderArray<double> X_end(myReader, "_rc_xe_corr.rc_xe_corr");
        TTreeReaderArray<double> Y_end(myReader, "_rc_ye.rc_ye");
        TTreeReaderArray<double> Z_end(myReader, "_rc_ze.rc_ze");
        TTreeReaderArray<double> flash(myReader, "_dt_flash.dt_flash");
        TTreeReaderArray<int> cathode(myReader, "_cathode.cathode");
        TTreeReaderArray<int> anode(myReader, "_anode.anode");
	long events = 0;
        long tracks = 0;
	while(myReader.Next()){
                //check to see if data is good. Hannah Rogers informed me of this check
                // all cathode data is good, want anode data with dt_flash>1
                for(int i=0; i<X_star.GetSize();i++){
                if(/*if it is a anode with flash greater than one*/( flash[i]<=1 && anode[i]>=1)|| cathode[i]>=1){
                        TTreeReader newReader("t0ana/_tree", f);
                        TTreeReaderArray<double> Y_star2(newReader, "_rc_ys.rc_ys");
                        TTreeReaderArray<double> Z_star2(newReader, "_rc_zs.rc_zs");
                        TTreeReaderArray<double> X_end2(newReader, "_rc_xe_corr.rc_xe_corr");
                        TTreeReaderArray<double> Y_end2(newReader, "_rc_ye.rc_ye");
                        TTreeReaderArray<double> Z_end2(newReader, "_rc_ze.rc_ze");
                        TTreeReaderArray<double> flash2(newReader, "_dt_flash.dt_flash");
                        TTreeReaderArray<int> cat2(newReader, "_cathode.cathode");
                        TTreeReaderArray<int> ano2(newReader, "_anode.anode");
                        }
                }

        }
        
        his->SetDirectory(0);
        xy->SetDirectory(0);
        xz->SetDirectory(0);
        yz->SetDirectory(0);
        f->Close();
        TFile *dump = new TFile("3dhist.root", "RECREATE");
        //his->Write();
        //xz->Write();
        //xy->Write();
        //his->Draw();
        //xz->Draw();
        //xy->Draw();
        dump->Close();
        //his->Draw("COLZ");
        gStyle->SetOptStat(kFALSE);
        yz->GetXaxis()->SetTitle("Z [cm]");
        yz->GetYaxis()->SetTitle("Y [cm]");
        xy->GetXaxis()->SetTitle("X [cm]");
        xy->GetYaxis()->SetTitle("Y [cm]");
        xz->GetXaxis()->SetTitle("X [cm]");
        xz->GetYaxis()->SetTitle("Z [cm]");
        
        yz->DrawNormalized("COLZ", events);
        //xz->Draw("COLZ");
        //cout <<"total loops: " << ind << endl;
        //cout << "total data: " << data << endl;
        printf("events: %ld\n", events );
        printf("tracks: %ld\n", tracks);
        //printf("\n");
        printf("IT WORKS!!!!!\n");
        return;
}

