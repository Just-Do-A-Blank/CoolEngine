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

    virtual bool GetAttachesToWeaponDamage();

    virtual bool GetKillsOnDrain();

    virtual int GetGain();

    virtual void SetValue(int value);

    virtual void SetMaxValue(int value);

    virtual void SetMinValue(int value);

    virtual void SetDefaultValue(int value);

    virtual void SetAttachesToWeaponDamage(bool value);

    virtual void SetKillsOnDrain(bool value);

    virtual void SetGain(int value);

private:
	int m_value;

	int m_maxValue;

	int m_minValue;

	int m_defaultValue;

	string m_key;
	
    /// <summary>
    /// True means this attaches to weapon damage
    /// </summary>
    bool m_attachToWeaponDamage;

    /// <summary>
    /// True means if this is min value then it kill the character
    /// </summary>
    bool m_resourceKillOnDrain;

    /// <summary>
    /// How much to gain every half second
    /// </summary>
    int m_valueGained;
};

