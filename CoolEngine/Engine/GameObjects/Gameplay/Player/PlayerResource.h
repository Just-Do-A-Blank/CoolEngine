#pragma once
class PlayerResource
{
public:
    friend class PlayerResourceInterface;

	PlayerResource(string key);

    string GetKey() const;

    virtual int GetValue();

    virtual int GetDefaultValue();

    virtual int GetMaxValue();

    virtual int GetMinValue();

    virtual void SetValue(int value);

    virtual void SetMaxValue(int value);

    virtual void SetMinValue(int value);

    virtual void SetDefaultValue(int value);

private:
	int m_value;

	int m_maxValue;

	int m_minValue;

	int m_defaultValue;

	string m_key;
	
};

