package Table;

public class MS {	
	private String _IMEI;
	private String _MSISDN;
	private String _UserName;
	private String _MSCCompany;
	private String _gsmMspSense;
	private String _gsmMsHeight;
	private String _gsmMspFout;
	private String _MZONE;
	
	private String IMEI;
	private String MSISDN;
	private String UserName;
	private String MSCCompany;
	private int gsmMspSense;
	private float gsmMsHeight;
	private float gsmMspFout;
	private String MZONE;

	public MS(String _IMEI, String _MSISDN, String _UserName, String _MSCCompany, 
			String _gsmMspSense, String _gsmMsHeight, String _gsmMspFout, String _MZONE){
		this._IMEI = _IMEI;
		this._MSISDN = _MSISDN;
		this._UserName = _UserName;
		this._MSCCompany = _MSCCompany;
		this._gsmMspSense = _gsmMspSense;
		this._gsmMsHeight = _gsmMsHeight;
		this._gsmMspFout = _gsmMspFout;
		this._MZONE = _MZONE;
		
		this.IMEI = _IMEI;
		this.MSISDN = _MSISDN;
		this.UserName = _UserName;
		this.MSCCompany = _MSCCompany;
		this.gsmMspSense = 0;
		this.gsmMsHeight = 0;
		this.gsmMspFout = 0;
		this.MZONE = _MZONE;
	}	

	public boolean checkRow(){
		try{
			gsmMspSense = Integer.parseInt(_gsmMspSense);
			gsmMsHeight = Float.parseFloat(_gsmMsHeight);
			gsmMspFout = Float.parseFloat(_gsmMspFout);
			if (gsmMspSense < 0 || gsmMsHeight < 0 || gsmMspFout < 0)
				return false;
			return true;
		} catch(NumberFormatException e){
			return false;
		}
	}

	public String getIMEI() {
		return IMEI;
	}

	public String getMSISDN() {
		return MSISDN;
	}

	public String getUserName() {
		return UserName;
	}

	public String getMSCCompany() {
		return MSCCompany;
	}

	public int getGsmMspSense() {
		return gsmMspSense;
	}

	public float getGsmMsHeight() {
		return gsmMsHeight;
	}

	public float getGsmMspFout() {
		return gsmMspFout;
	}

	public String getMZONE() {
		return MZONE;
	}
}
