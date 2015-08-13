package Table;

public class Anrenna {
	private String _CellID;
	private String _AnrennaHigh;
	private String _HalfPAngle;
	private String _MaxAttenuation;
	private String _Gain;
	private String _AntTilt;
	private String _pt;
	private String _MsPwr;
	
	private int CellID;
	private int AnrennaHigh;
	private int HalfPAngle;
	private int MaxAttenuation;
	private int Gain;
	private int AntTilt;
	private int pt;
	private int MsPwr;
	
	public Anrenna(String _CellID, String _AnrennaHigh, String _HalfPAngle, String _MaxAttenuation, 
			String _Gain, String _AntTilt, String _pt, String _MsPwr){
		this._CellID = _CellID;
		this._AnrennaHigh = _AnrennaHigh;
		this._HalfPAngle = _HalfPAngle;
		this._MaxAttenuation = _MaxAttenuation;
		this._Gain = _Gain;
		this._AntTilt = _AntTilt;
		this._pt = _pt;
		this._MsPwr = _MsPwr;
		
		this.CellID = 0;
		this.AnrennaHigh = 0;
		this.HalfPAngle = 0;
		this.MaxAttenuation = 0;
		this.Gain = 0;
		this.AntTilt = 0;
		this.pt = 0;
		this.MsPwr = 0;
	}
	
	public boolean checkRow(){
		try{
			CellID = Integer.parseInt(_CellID);
			AnrennaHigh = Integer.parseInt(_AnrennaHigh);
			HalfPAngle = Integer.parseInt(_HalfPAngle);
			MaxAttenuation = Integer.parseInt(_MaxAttenuation);
			Gain = Integer.parseInt(_Gain);
			AntTilt = Integer.parseInt(_AntTilt);
			pt = Integer.parseInt(_pt);
			MsPwr = Integer.parseInt(_MsPwr);
			if (CellID < 0 || AnrennaHigh < 0 || HalfPAngle < 0)
				return false;
			return true;
		} catch(NumberFormatException e){
			return false;
		}
	}
	
	public int getCellID() {
		return CellID;
	}

	public int getAnrennaHigh() {
		return AnrennaHigh;
	}

	public int getHalfPAngle() {
		return HalfPAngle;
	}

	public int getMaxAttenuation() {
		return MaxAttenuation;
	}

	public int getGain() {
		return Gain;
	}

	public int getAntTilt() {
		return AntTilt;
	}

	public int getPt() {
		return pt;
	}

	public int getMsPwr() {
		return MsPwr;
	}
}
