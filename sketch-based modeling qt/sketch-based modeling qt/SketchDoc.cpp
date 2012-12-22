#include "SketchDoc.h"
#include "sketchinterface.h"
#include "OBJHandle.h"
#include <QMessageBox>


SketchDoc::SketchDoc(SketchInterface* pParentIn)
{
	this->bShowDualMesh=false;
	this->bShowPrimalMesh=true;
	this->pParent=pParentIn;
	NewDocument();
}

SketchDoc::~SketchDoc(void)
{
}

void SketchDoc::NewDocument()
{
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	//CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	//CKWResearchWorkView* pView=(CKWResearchWorkView*)GetView(RUNTIME_CLASS(CKWResearchWorkView));
	//if (pMain==NULL)
	//{
	//	pView->Reset(true);
	//}
	//else
	//{
	//	pView->Reset(false);
	//	CMenu* pMenu=pMain->GetMenu();
	//	pMenu->CheckMenuItem(ID_VIEW_3DAXISON,MF_UNCHECKED);	
	//	this->bAxis=false;
	//	this->bShowPrimalMesh=true;
	//	pMenu->CheckMenuItem(ID_VIEW_PRIMALMESH,MF_CHECKED);	
	//	//		this->iViewStyle=1;
	//}

	this->Mesh.clear();
	this->MeshEditing.Init(this);
	//this->MeshDeformation.Init(this);
	//this->MeshExtrusion.Init(this);
	//this->MeshCutting.Init(this);
	//this->MeshSmoothing.Init(this);
	this->MeshCreation.Init(this);
	this->Test.Init(this);
	OnModeCreation();


	float pos[4] = { 0, 0, 1, 0};
	SetLightPos(pos);

	this->iManipMode=VIEW_SELECTION_MODE;
	this->iRBSelName=NONE_SELECTED;
	this->iLBSelName=NONE_SELECTED;
	this->iColorMode=COLOR_ORIGINAL;
	vecDefaultColor.clear();
	vecDefaultColor.push_back(0.5);
	vecDefaultColor.push_back(0.9);
	vecDefaultColor.push_back(0.4);
	vecDefaultColor.push_back(1.0);
	this->iViewStyle=SMOOTH_VIEW;
	this->bAxis=false;
	this->iRenderPreMesh=MESH_PREVIEW;
	//init control panel(general tab)
	//CControlPanel* pCP=(CControlPanel*)(this->GetView(RUNTIME_CLASS(CControlPanel)));
	//if (pCP->GetCPGeneral()!=NULL)
	//{
	//	pCP->GetCPGeneral()->Init();
	//}

	this->pParent->update();
}

void SketchDoc::OpenDocument(QString qDocName)
{
	bool bScale,bCenter;
	bScale=bCenter=true;

	QMessageBox msgBox;
	msgBox.setText("Scale the model size?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	switch (msgBox.exec())
	{
	case QMessageBox::Cancel:
		return;
	case QMessageBox::No:
		bScale=false;
		break;
	default:
		break;
	}

	msgBox.setText("Center the model?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	switch (msgBox.exec())
	{
	case QMessageBox::Cancel:
		return;
	case QMessageBox::No:
		bCenter=false;
		break;
	default:
		break;
	}


	//	OBJHandle::glmReadOBJ((char *)lpszPathName,this->Mesh,bScale,bCenter);
	OBJHandle::glmReadOBJNew(qDocName.toUtf8().constData(),this->Mesh,bScale,bCenter,this->vecDefaultColor);

	if(this->Mesh.empty())
	{
		msgBox.setText("Read File Error!");
		msgBox.exec();
		return;
	}

	// TODO: add loading code here
	this->MeshEditing.Init(this);
	//this->MeshDeformation.Init(this);
	//this->MeshExtrusion.Init(this);
	//this->MeshCutting.Init(this);
	//this->MeshSmoothing.Init(this);
	this->MeshCreation.Init(this);
	this->Test.Init(this);
	//OnModeDeformation();

	this->iManipMode=VIEW_SELECTION_MODE;
	this->iRBSelName=NONE_SELECTED;
	this->iLBSelName=NONE_SELECTED;
	this->iColorMode=COLOR_ORIGINAL;
	this->iRenderPreMesh=MESH_EXIST_VIEW;

	this->pParent->update();
}

void SketchDoc::SaveDocument(QString qDocName)
{
	// TODO: Add your specialized code here and/or call the base class
	if (this->Mesh.empty())
	{
		QMessageBox msgBox;
		msgBox.setText("Save File Error!");
		msgBox.exec();
		return;
	}

	std::ofstream out(qDocName.toUtf8().constData(),ios_base::out | ios_base::trunc);
	if(!out)  
	{  
		QMessageBox msgBox;
		msgBox.setText("Create File Error!");
		msgBox.exec();
	}

	print_polyhedron_wavefront(out,Mesh);

	//SetModifiedFlag(FALSE);
}

vector<vector<Point_3> >& SketchDoc::GettestvecvecNewEdgeVertexPos()
{
	return this->testvecvecNewEdgeVertexPos;
}

bool SketchDoc::JudgeEditPlane()
{
//	if (this->MeshDeformation.GetDeformCurvePoints().empty())
//	{
//		return false;
//	}
	return true;
}

int SketchDoc::GetCurrentDesiredPointsPos(vector<HPCPoint> & DesiredPointsPosition)
{
	//return this->MeshDeformation.GetCurrentDesiredPointsPos(DesiredPointsPosition);
	return 0;
}

void SketchDoc::OnModeCreation()
{
	// TODO: Add your command handler code here
	if (this->iEditMode==CREATION_MODE)
	{
		return;
	}
	else
	{
		this->iEditMode=CREATION_MODE;
	}

	//CControlPanel* pCP=(CControlPanel*)(this->GetView(RUNTIME_CLASS(CControlPanel)));
	//if (pCP->GetCPCreation()!=NULL)
	//{
	//	pCP->GetCPTab().SetCurFocus(CREATION_MODE+1);//1,tab for creation
	//}

	this->pParent->update();
}

void SketchDoc::OnModeEditing()
{
	// TODO: Add your command handler code here
	if (this->iEditMode==EDITING_MODE)
	{
		return;
	}
	else
	{
		this->iEditMode=EDITING_MODE;
	}

	//CControlPanel* pCP=(CControlPanel*)(this->GetView(RUNTIME_CLASS(CControlPanel)));
	//if (pCP->GetCPEditing()!=NULL)
	//{
	//	pCP->GetCPTab().SetCurFocus(EDITING_MODE+1);//2,tab for editing
	//}
	this->pParent->update();
}

void SketchDoc::OnModeDeformation()
{
	// TODO: Add your command handler code here
	if (this->iEditMode==DEFORMATION_MODE)
	{
		return;
	}
	else
	{
		this->iEditMode=DEFORMATION_MODE;
	}

	//CControlPanel* pCP=(CControlPanel*)(this->GetView(RUNTIME_CLASS(CControlPanel)));
	//if (pCP->GetCPDeformation()!=NULL)
	//{
	//	pCP->GetCPTab().SetCurFocus(DEFORMATION_MODE+1);//3,tab for deformation
	//}
	this->pParent->update();
}

void SketchDoc::OnModeExtrusion()
{
	// TODO: Add your command handler code here
	if (this->iEditMode==EXTRUSION_MODE)
	{
		return;
	}
	else
	{
		this->iEditMode=EXTRUSION_MODE;
	}

	//CControlPanel* pCP=(CControlPanel*)(this->GetView(RUNTIME_CLASS(CControlPanel)));
	//if (pCP->GetCPExtrusion()!=NULL)
	//{
	//	pCP->GetCPTab().SetCurFocus(EXTRUSION_MODE+1);//4,tab for extrusion
	//}
	this->pParent->update();
}

void SketchDoc::OnModeCutting()
{
	// TODO: Add your command handler code here
	if (this->iEditMode==CUTTING_MODE)
	{
		return;
	}
	else
	{
		this->iEditMode=CUTTING_MODE;
	}

	//CControlPanel* pCP=(CControlPanel*)(this->GetView(RUNTIME_CLASS(CControlPanel)));
	//if (pCP->GetCPCutting()!=NULL)
	//{
	//	pCP->GetCPTab().SetCurFocus(CUTTING_MODE+1);//5,tab for cutting
	//}
	this->pParent->update();
}

void SketchDoc::OnModeSmoothing()
{
	// TODO: Add your command handler code here
	if (this->iEditMode==SMOOTHING_MODE)
	{
		return;
	}
	else
	{
		this->iEditMode=SMOOTHING_MODE;
	}

	//CControlPanel* pCP=(CControlPanel*)(this->GetView(RUNTIME_CLASS(CControlPanel)));
	//if (pCP->GetCPSmoothing()!=NULL)
	//{
	//	pCP->GetCPTab().SetCurFocus(SMOOTHING_MODE+1);//6,tab for smoothing
	//}
	this->pParent->update();
}

void SketchDoc::OnModeTest()
{
	// TODO: Add your command handler code here
	if (this->iEditMode==TEST_MODE)
	{
		return;
	}
	else
	{
		this->iEditMode=TEST_MODE;
	}

	//CControlPanel* pCP=(CControlPanel*)(this->GetView(RUNTIME_CLASS(CControlPanel)));
	//if (pCP->GetCPTest()!=NULL)
	//{
	//	pCP->GetCPTab().SetCurFocus(TEST_MODE+1);//7,tab for test
	//}
	this->pParent->update();
}

void SketchDoc::OnViewSelectMode()
{
	this->iManipMode=VIEW_SELECTION_MODE;
}

void SketchDoc::OnSketchMode()
{
	this->iManipMode=SKETCH_MODE;
}

void SketchDoc::OnHelpTest()
{

}
