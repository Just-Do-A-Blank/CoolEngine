#pragma once
class PlayerResource
{
public:
	PlayerResource(string key);

	string GetKey() const;

	virtual int GetValue() const;

	virtual int GetMaxValue() const;

	virtual void SetValue(int value);

	virtual void SetMaxValue(int value);

	virtual int GetMinValue()const;

	virtual void SetMinValue(int value);

private:

	int m_value;

	int m_maxValue;

	int m_minValue;

	string m_key;
};

