#include <iostream>
#include <math.h>
#include "TFile.h"
#include "TTree.h"

void _3dhist(){
	TFile *f = TFile::Open("T0RecoSCE_ana.root");
	if(f==0){
		printf("Error: cannot open file\n");
		return;
		}
	TH3F* his = new TH3F("hPos", "Number of Events", 34, -380, 380, 30, 0, 600, 35, 0, 700);
	TH2F* xy = new TH2F("", "XY-Projection [560cm < Z < 700cm]", 34, -380, 380, 30, 0 ,600); 
	TH2F* xz = new TH2F("", "XZ Projection [480cm < Y < 600cm]", 34, -380, 380, 35, 0 ,700);
	TH2F* yz = new TH2F("", "ZY Projection [-76cm < X < -228cm]",  35, 0, 700, 30, 0, 600);
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
	TTreeReaderArray<int> TrackNum(myReader, "_track.track");
	long events = 0;
	long tracks = 0;
	//int ind=0;
	//int data=0;
	while(myReader.Next()){
		//check to see if data is good. Hannah Rogers informed me of this check
		// all cathode data is good, want anode data with dt_flash>1
		for(int i=0; i<X_star.GetSize();i++){
		if(TrackNum[i]==0) events++;
		if(/*if it is a anode with flash greater than one*/( flash[i]<=1 && anode[i]>=1)|| cathode[i]>=1){
			//data++;
			tracks++;
			double xcur = X_star[i];
                        double ycur = Y_star[i];
                        double zcur = Z_star[i];
                        //ycur = (6.0/6.08)*(ycur+.2);
                        //zcur = (7.2/6.97)*(zcur+.8);		
			double xslo = (X_end[i] - X_star[i])/(pow((pow((Y_end[i]-Y_star[i]),2.0)+pow((Z_end[i]-Z_star[i]),2.0)+pow((X_end[i]-X_star[i]),2.0)),.5));	
			double yslo = (Y_end[i] - Y_star[i])/(pow((pow((X_end[i]-X_star[i]),2.0)+pow((Z_end[i]-Z_star[i]),2.0)+pow((Y_end[i]-Y_star[i]),2.0)),.5));
			double zslo = (Z_end[i] - Z_star[i])/(pow((pow((Y_end[i]-Y_star[i]),2.0)+pow((X_end[i]-X_star[i]),2.0)+pow((Z_end[i]-Z_star[i]),2.0)),.5));
			//double xslo = (X_end[i] - X_star[i])/1000.0;
			//double yslo = (Y_end[i] - Y_star[i])/1000.0;
			//double zslo = (Z_end[i] - Z_star[i])/1000.0;

			//convert slopes to detector frame
			double xnex = X_star[i] + (xslo);
			double ynex = Y_star[i] + (yslo);
			double znex = Z_star[i] + (zslo);
			//convert values to dectetor frame
			//ycur = (6.0/6.08)*(ycur+.2);
                        //zcur = (7.2/6.97)*(zcur+.8);
			//ynex = (6.0/6.08)*(ynex+.2);
                        //xnex = (7.2/6.97)*(znex+.8);
			if(xcur> -380 && xcur < -228)
				yz->Fill(zcur, ycur);  
			his->Fill(xcur, ycur, zcur);
			if(zcur>=560 && zcur<= 700)
				xy->Fill(xcur, ycur);
			if(ycur>=480 && ycur<=600)
				xz->Fill(xcur, zcur);
				do{
					if(((int)xnex/20 != (int)xcur/20)||( (int)ynex/20 != (int)ycur/20)||( (int)znex/20 != (int)zcur/20)){
						if(xnex> -380 && xnex < -228)  
                                			yz->Fill(znex, ynex);
						if(ycur>=480 && ycur<=600)
							xz->Fill(xnex, znex);
						if(znex>=560 && znex<= 700)
							xy->Fill(xnex, ynex);
						his->Fill(xnex, ynex, znex);
					}
					if((int)xcur/20!=(int)X_end[i]/20){
                                        xcur=xnex;
					xnex+=(xslo);
                                        }if((int)ycur/20!=(int)Y_end[i]/20){
                                        ycur=ynex;
					ynex+=(yslo);
                                        }if((int)zcur/20!=(int)Z_end[i]/20){
					zcur=znex;
					znex+=zslo;
					}
				}
				while(((int)xcur/20!=(int)X_end[i]/20)&&((int)ycur/20!=(int)Y_end[i]/20)&&((int)zcur/20!=(int)Z_end[i]/20));
				//while( (xcur<360 && xcur>-360) && (ycur<600 && ycur>0) && (zcur<700 && zcur>0) );
			//his->Fill(xcur, ycur, zcur);
                        //xy->Fill(xcur, ycur);
                        //xz->Fill(xcur, zcur);
			}
		//ind++;
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
