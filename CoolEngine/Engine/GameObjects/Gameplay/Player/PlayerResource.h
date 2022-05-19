#pragma once
class PlayerResource
{
public:
	PlayerResource(string key);

	int m_value;

	int m_maxValue;

	int m_minValue;

	/*string GetKey() const;

	virtual int& GetValue();

	virtual int& GetMaxValue();

	virtual void SetValue(int value);

	virtual void SetMaxValue(int value);

	virtual int& GetMinValue();

	virtual void SetMinValue(int value);*/

private:

	string m_key;
	
};

