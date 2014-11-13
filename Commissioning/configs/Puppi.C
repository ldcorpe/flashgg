{
	//TFile *_file0 = TFile::Open("tree.root");//Open File
	//TTree *eventTree = (TTree*)flashggTreeMakerWithTagSorter->Get("flashggTreeMakerWithTagSorter"); //access tree
	TTree *eventTree = (TTree*)flashggTreeMakerWithTagSorter->Get("flashggTreeWithTagSorter"); //access tree

	/*	struct eventInfo { //define struct to house the values held in the branch
			float genVertexZ;
			float zerothVertexZ;
			float diphotonVertexZ;
			float higgsPt;
			};*/

//	eventInfo eInfo; 


	float dijet_LeadJPt;
	float dijet_SubJPt;
	int leadjet_genmatch;
	int subljet_genmatch;
	int flash_VBFTag_Category;
	float pu_n;


	//TBranch eventBranch;
	/*	TBranch dijet_LeadJPt;
			TBranch dijet_SubJPt;
			TBranch leadjet_genmatch;
			TBranch subljet_genmatch;
			TBranch flash_VBFTag_Category;*/
	// trick branch into filling all three values of the struct by asking it to fill the first. The other three get filled automatically. 
	//eventTree->SetBranchAddress("eventBranch", &eInfo.genVertexZ); 
	eventTree->SetBranchAddress("dijet_LeadJPt", &dijet_LeadJPt); 
	eventTree->SetBranchAddress("dijet_SubJPt", &dijet_SubJPt); 
	eventTree->SetBranchAddress("leadjet_genmatch", &leadjet_genmatch); 
	eventTree->SetBranchAddress("subljet_genmatch", &subljet_genmatch); 
	eventTree->SetBranchAddress("flash_VBFTag_Category", &flash_VBFTag_Category); 
	eventTree->SetBranchAddress("pu_n", &pu_n); 

	TH1F *ptnum0 = new TH1F("ptnum0","ptnum0",50,0,250); //define histos for numerator and denominator
	TH1F *ptden0 = new TH1F("ptden0","ptden0",50,0,250);
	TH1F *pu_denVBF = new TH1F("pu_denVBF","pu_denVBF",50,0,250);
	TH1F *pu_numVBF = new TH1F("pu_numVBF","pu_numVBF",50,0,250);
//	TH1F *ptnumLeg = new TH1F("ptnumLeg","ptnumLeg",50,0,250);
//	TH1F *ptdenLeg = new TH1F("ptdenLeg","ptdenLeg",50,0,250);


	int counter =0;

	for( Long64_t loop = 0 ; loop < eventTree->GetEntries(); loop++ )
	{
		eventTree->GetEntry(loop);
		//std::cout << flash_VBFTag_Category  <<std::endl;
		pu_denVBF->Fill(pu_n);
		if(flash_VBFTag_Category != -1){
		
		pu_numVBF->Fill(pu_n);
	//	ptnumVBF->Fill(dijet_LeadJPt);
		
		}
	//	std::cout << eInfo.zerothVertexZ << std::endl;
		if((dijet_LeadJPt <0) || (dijet_SubJPt <0)) {counter++; continue;} 

		ptden0->Fill(dijet_LeadJPt);
		ptden0->Fill(dijet_SubJPt);
		//ptdenLeg->Fill(eInfo.higgsPt);
		std::cout << leadjet_genmatch << "	" << subljet_genmatch << std::endl;

		if(leadjet_genmatch ==1) 		{
			ptnum0->Fill(dijet_LeadJPt);
		}
		if(subljet_genmatch ==1) 		{
			ptnum0->Fill(dijet_SubJPt);
		}
		

	}

	TGraphAsymmErrors *graph0 = new TGraphAsymmErrors(ptnum0,ptden0);
	TGraphAsymmErrors *graphVBF = new TGraphAsymmErrors(pu_numVBF,pu_denVBF);
//	TGraphAsymmErrors *graphLeg = new TGraphAsymmErrors(ptnumLeg,ptdenLeg);
////	graphLeg->SetMarkerColor(kRed); 
//	graphLeg->SetLineColor(kRed);
	//graphLeg->Draw("P");
	
	TFile *t = new TFile("histo.root","recreate");
	graph0->Draw("AP"); 
	graph0->Write();
	c1->SaveAs("matchedJets.pdf");
	graphVBF->Draw("AP"); 
	graphVBF->Write();
	c1->SaveAs("VBFTagged.pdf");

	std:: cout << "Statistics: " << eventTree->GetEntries() << " events." << std::endl;
	std:: cout << "skipped " << counter << " events." << std::endl;
}
