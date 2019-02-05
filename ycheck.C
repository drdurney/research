void ycheck(){
        TFile *f = TFile::Open("T0RecoSCE_ana.root");
        if(f==0){
                printf("Error: cannot open file\n");
               return;
                }
        //TH3F* his = new TH3F("hPos", "Number of Events", 34, -380, 380, 30, 0, 600, 35, 0, 700);
        TH1F* y = new TH1F("", "Y projection", 30, 0 ,600);
	TH1F* ys = new TH1F("", "Y Start", 30, 0, 600);
	TH1F* ye = new TH1F("", "Y end", 30, 0, 600);
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
	while(myReader.Next()){
                //check to see if data is good. Hannah Rogers informed me of this check
                // all cathode data is good, want anode data with dt_flash>1
                for(int i=0; i<Y_star.GetSize();i++){
	if(/*if it is a anode with flash greater than one( flash[i]<=1 && anode[i]>=1) ||*/ cathode[i]>=1){
                        double ycur = Y_star[i];
			int yind = (int)Y_star[i]/20;
			y->Fill(ycur);
			ys->Fill(Y_star[i]);
			ye->Fill(Y_end[i]);
			do{
				if((int)ycur/20!=yind){
					y->Fill(ycur);
					yind=(int)ycur/20;
				}
			if(Y_end[i]-Y_star[i]<0)
				ycur--;
			else
				ycur++;		
				
			}while((int)ycur!=(int)Y_end[i]);
                                               
                        }
                }

        }

        y->SetDirectory(0);
	ys->SetDirectory(0);
	ye->SetDirectory(0);
	f->Close();
        TFile *dump = new TFile("3dhist.root", "RECREATE");
        dump->Close();
	y->Draw();
	return;
}

