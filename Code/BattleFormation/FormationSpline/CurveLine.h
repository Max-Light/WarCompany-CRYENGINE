#pragma once

#include "ICurveLine.h"

class CCurveLine : public ICurveLine
{
public:
	CCurveLine(CAnchorPoint* pOutAnchor, CAnchorPoint* pInAnchor, const ECurveMode& curveMode = ECurveMode::Linear)
		: m_outAnchor(pOutAnchor)
		, m_inAnchor(pInAnchor)
		, m_curveMode(curveMode)
	{
	}
	virtual ~CCurveLine() = default;

	// ICurveLine
	virtual ECurveMode GetCurveMode() const override { return m_curveMode; }
	virtual void ApplyCurveMode(const ECurveMode& mode) override;
	// ~ICurveLine

	// Return the outgoing anchor
	CAnchorPoint* GetOutAnchor() const { return m_outAnchor; }

	// Return the incoming anchor
	CAnchorPoint* GetInAnchor() const { return m_inAnchor; }

	// Sets the outgoing anchor
	void SetOutAnchor(CAnchorPoint* pAnchor) { m_outAnchor = pAnchor; }

	// Sets the incoming anchor
	void SetInAnchor(CAnchorPoint* pAnchor) { m_inAnchor = pAnchor; }
private:
	CAnchorPoint* m_outAnchor = nullptr;
	CAnchorPoint* m_inAnchor = nullptr;
	ECurveMode m_curveMode;
};