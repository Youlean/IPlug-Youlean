#include "IControlGroup.h"
#include "IControl.h"
#include "IPlugGUIResize.h"



IControlGroup::IControlGroup(IPlugBase * pPlug)
{
	mPlug = pPlug;

	if (mPlug->GetGUIResize())
	{
		for (int i = 0; i < mPlug->GetGUIResize()->GetViewModeSize(); i++)
		{
			originalGroupRECTs.push_back(DRECT());
		}
	}
	else
	{
		originalGroupRECTs.push_back(DRECT());
	}
}

IControl* IControlGroup::AddControl(IControl* pControl)
{
	pControl->AttachIControlGroup(this);

	controlProps.push_back(IControlProperties(pControl, *pControl->GetDrawRECT(), *pControl->GetTargetRECT()));
	//if (moveControlRelativeToGroup) MoveControlRelativeToGroup(pControl);

	if (mPlug->GetGUIResize())
	{
		for (int i = 0; i < mPlug->GetGUIResize()->GetViewModeSize(); i++)
		{
			groupRECT = originalGroupRECTs[i] = originalGroupRECTs[i].Union(pControl->GetNonScaledDrawRECT());
		}
	}
	else
	{
		groupRECT = originalGroupRECTs[0] = originalGroupRECTs[0].Union(pControl->GetNonScaledDrawRECT());
	}

	return pControl;
}

IControlGroup * IControlGroup::CreateSubgroup()
{
	IControlGroup* subGroup = new IControlGroup(mPlug);

	subGroup->SetSuperGroup(this);

	controlSubgroups.push_back(subGroup);
	return controlSubgroups.back();
}

void IControlGroup::SetSuperGroup(IControlGroup * SuperGroup)
{
	superGroup = SuperGroup;
}

IControlGroup * IControlGroup::GetSuperGroup()
{
	return superGroup;
}

DRECT IControlGroup::GetGroupRECT()
{
	return groupRECT;
}

void IControlGroup::SetGroupRECT(DRECT GroupRECT)
{
	groupRECT = GroupRECT;

	MoveAllControlsRelativeToGroup();
}

bool IControlGroup::IsGroupHidden()
{
	return groupHidden;
}

bool IControlGroup::IsGroupHiddenInSuperGroups()
{
	IControlGroup* superGroup = this;
	bool hidden = false;

	while (superGroup->GetSuperGroup())
	{
		superGroup = superGroup->GetSuperGroup();
		hidden = superGroup->IsGroupHidden();

		if (hidden) return true;
	}

	return false;
}

void IControlGroup::MoveGroup(double x, double y)
{
	double relativeX = x - groupRECT.L;
	double relativeY = y - groupRECT.T;

	double width = groupRECT.W();
	double height = groupRECT.H();

	groupRECT.L = x;
	groupRECT.T = y;
	groupRECT.R = x + width;
	groupRECT.B = y + height;

	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroup(relativeX, relativeY);
	}
}

void IControlGroup::MoveGroupHorizontally(double x)
{
	double relativeX = x - groupRECT.L;
	double width = groupRECT.W();

	groupRECT.L = x;
	groupRECT.R = x + width;

	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroupHorizontally(relativeX);
	}
}

void IControlGroup::MoveGroupVertically(double y)
{
	double relativeY = y - groupRECT.T;
	double height = groupRECT.H();

	groupRECT.T = y;
	groupRECT.B = y + height;

	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroupVertically(relativeY);
	}
}

void IControlGroup::MoveGroupLeftEdge(double L)
{
	double relativeL = L - groupRECT.L;

	groupRECT.L = L;
	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroupLeftEdge(relativeL);
	}
}

void IControlGroup::MoveGroupTopEdge(double T)
{
	double relativeT = T - groupRECT.T;
	groupRECT.T = T;
	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroupTopEdge(relativeT);
	}
}

void IControlGroup::MoveGroupRightEdge(double R)
{
	double relativeR = R - groupRECT.R;

	groupRECT.R = R;
	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroupRightEdge(relativeR);
	}
}

void IControlGroup::MoveGroupBottomEdge(double B)
{
	double relativeB = B - groupRECT.B;

	groupRECT.B = B;
	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroupBottomEdge(relativeB);
	}
}

void IControlGroup::RelativelyMoveGroup(double x, double y)
{
	groupRECT.L += x;
	groupRECT.T += y;
	groupRECT.R += x;
	groupRECT.B += y;

	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroup(x, y);
	}
}

void IControlGroup::RelativelyMoveGroupHorizontally(double x)
{
	groupRECT.L += x;
	groupRECT.R += x;

	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroupHorizontally(x);
	}
}

void IControlGroup::RelativelyMoveGroupVertically(double y)
{
	groupRECT.T += y;
	groupRECT.B += y;

	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroupVertically(y);
	}
}

void IControlGroup::RelativelyMoveGroupLeftEdge(double L)
{
	groupRECT.L += L;
	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroupLeftEdge(L);
	}
}

void IControlGroup::RelativelyMoveGroupTopEdge(double T)
{
	groupRECT.T += T;
	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroupTopEdge(T);
	}
}

void IControlGroup::RelativelyMoveGroupRightEdge(double R)
{
	groupRECT.R += R;
	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroupRightEdge(R);
	}
}

void IControlGroup::RelativelyMoveGroupBottomEdge(double B)
{
	groupRECT.B += B;
	MoveAllControlsRelativeToGroup();

	// Move subgroups
	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->RelativelyMoveGroupBottomEdge(B);
	}
}

int IControlGroup::GetNumberOfSubgroups()
{
	return controlSubgroups.size();
}

int IControlGroup::GetNumberOfControls()
{
	return controlProps.size();
}

int IControlGroup::GetNumberOfControlsIncludingSubgroups()
{
	int numberOfControls = 0;

	// Gets number of attached IControls in a group
	numberOfControls = controlProps.size();

	for (int i = 0; i < controlSubgroups.size(); i++)
		numberOfControls += controlSubgroups[i]->GetNumberOfControls();

	return numberOfControls;
}

double IControlGroup::L() { return groupRECT.L; }

double IControlGroup::T() { return groupRECT.T; }

double IControlGroup::R() { return groupRECT.R; }

double IControlGroup::B() { return groupRECT.B; }

double IControlGroup::W() { return groupRECT.W(); }

double IControlGroup::H() { return groupRECT.H(); }

inline double IControlGroup::MW() const { return 0.5 * (groupRECT.L + groupRECT.R); }

inline double IControlGroup::MH() const { return 0.5 * (groupRECT.T + groupRECT.B); }

IControlGroup * IControlGroup::GetSubgroup(int index)
{
	return controlSubgroups[index];
}

IControl * IControlGroup::GetControl(int index)
{
	return controlProps[index].control;
}

IControl * IControlGroup::GetControlIncludingSubgroups(int index)
{
	if (index < controlProps.size()) return controlProps[index].control;
	else index -= controlProps.size();

	int i = 0;
	for (; index >= controlSubgroups[i]->GetNumberOfControls(); i++)
		index -= controlSubgroups[i]->GetNumberOfControls();

	return controlSubgroups[i]->GetControl(index);
}

void IControlGroup::HideControls()
{
	for (int i = 0; i < controlProps.size(); i++)
	{
		IControl *pControl = controlProps[i].control;

		if (pControl->GetGUIResize() && pControl->GetGUIResize()->IsAttachedToIPlugBase()) pControl->GetGUIResize()->HideControl(pControl);
		else controlProps[i].control->Hide(true);
	}
}

void IControlGroup::HideControlsWithSubgroups()
{
	HideControls();

	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->HideControlsWithSubgroups();
	}
}

void IControlGroup::ShowControls()
{
	for (int i = 0; i < controlProps.size(); i++)
	{
		IControl *pControl = controlProps[i].control;

		if (pControl->GetGUIResize() && pControl->GetGUIResize()->IsAttachedToIPlugBase()) pControl->GetGUIResize()->ShowControl(pControl);
		else controlProps[i].control->Hide(false);
	}
}

void IControlGroup::ShowControlsWithSubgroups()
{
	ShowControls();

	for (int i = 0; i < controlSubgroups.size(); i++)
	{
		controlSubgroups[i]->ShowControlsWithSubgroups();
	}
}

void IControlGroup::UpdateVisibilityStatusOfAttachedControls()
{
	if (IsGroupHidden() || IsGroupHiddenInSuperGroups())
	{
		HideControlsWithSubgroups();
	}
	else
	{
		ShowControls();

		for (int i = 0; i < controlSubgroups.size(); i++)
		{
			controlSubgroups[i]->UpdateVisibilityStatusOfAttachedControls();
		}
	}
}

void IControlGroup::HideGroup()
{
	groupHidden = true;
	UpdateVisibilityStatusOfAttachedControls();
}

void IControlGroup::ShowGroup()
{
	groupHidden = false;
	UpdateVisibilityStatusOfAttachedControls();
}

void IControlGroup::MoveSubgroupRelativeToGroup(IControlGroup * pControlGroup)
{
	DRECT subgroupRECT = pControlGroup->GetGroupRECT();

	subgroupRECT.L += groupRECT.L;
	subgroupRECT.T += groupRECT.T;
	subgroupRECT.R += groupRECT.L;
	subgroupRECT.B += groupRECT.T;

	pControlGroup->SetGroupRECT(subgroupRECT);
}

void IControlGroup::MoveControlRelativeToGroup(IControl* pControl)
{
	if (mPlug->GetGUIResize() && mPlug->GetGUIResize()->IsAttachedToIPlugBase())
	{
		int currentViewMode = pControl->GetGUIResize()->GetViewMode();
		DRECT *originalGroupRECT = &originalGroupRECTs[currentViewMode];

		DRECT originalDrawRECT = pControl->GetGUIResize()->GetOriginalDrawRECTForControl(pControl);		
		pControl->GetGUIResize()->MoveControl(pControl, groupRECT.L + (originalDrawRECT.L - originalGroupRECT->L), groupRECT.T + (originalDrawRECT.T - originalGroupRECT->T), drawAreaOnly);

		DRECT originalTargetRECT = pControl->GetGUIResize()->GetOriginalTargetRECTForControl(pControl);
		pControl->GetGUIResize()->MoveControl(pControl, groupRECT.L + (originalTargetRECT.L - originalGroupRECT->L), groupRECT.T + (originalTargetRECT.T - originalGroupRECT->T), targetAreaOnly);
	}
	else
	{
		int propsIndex = FindIndexOfPropertiesForControl(pControl);
		if (propsIndex < 0) return;

		IControlProperties *props = &controlProps[propsIndex];

		DRECT *originalGroupRECT = &originalGroupRECTs[0];

		pControl->GetDrawRECT()->L = int(groupRECT.L + (props->originalDrawRECT.L - originalGroupRECT->L));
		pControl->GetDrawRECT()->T = int(groupRECT.T + (props->originalDrawRECT.T - originalGroupRECT->T));
		pControl->GetDrawRECT()->R = int(groupRECT.L + (props->originalDrawRECT.R - originalGroupRECT->L));
		pControl->GetDrawRECT()->B = int(groupRECT.T + (props->originalDrawRECT.B - originalGroupRECT->T));

		*pControl->GetNonScaledDrawRECT() = *pControl->GetDrawRECT();

		pControl->GetTargetRECT()->L = int(groupRECT.L + (props->originalTargetRECT.L - originalGroupRECT->L));
		pControl->GetTargetRECT()->T = int(groupRECT.T + (props->originalTargetRECT.T - originalGroupRECT->T));
		pControl->GetTargetRECT()->R = int(groupRECT.L + (props->originalTargetRECT.R - originalGroupRECT->L));
		pControl->GetTargetRECT()->B = int(groupRECT.T + (props->originalTargetRECT.B - originalGroupRECT->T));
	}
}

void IControlGroup::MoveAllControlsRelativeToGroup()
{
	for (int i = 0; i < controlProps.size(); i++)
	{
		MoveControlRelativeToGroup(controlProps[i].control);
	}

	// Update original draw rect
	if (mPlug->GetGUIResize())
	{
		int currentViewMode = mPlug->GetGUIResize()->GetViewMode();
		DRECT *originalGroupRECT = &originalGroupRECTs[currentViewMode];
		*originalGroupRECT = groupRECT;
	}
	else
	{
		DRECT *originalGroupRECT = &originalGroupRECTs[0];
		*originalGroupRECT = groupRECT;
	}
}

int IControlGroup::FindIndexOfPropertiesForControl(IControl * pControl)
{
	for (int i = 0; i < controlProps.size(); i++)
	{
		if (controlProps[i].control == pControl) return i;
	}

	return -1;
}
