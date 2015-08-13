package Table;

public class MSC {	
	private String _MscID;
	private String _MscName;
	private String _MscCompany;
	private String _MscLongitude;
	private String _MscLatitude;
	private String _MscAltitude;
	
	private String MscID;
	private String MscName;
	private String MscCompany;
	private float MscLongitude;
	private float MscLatitude;
	private int MscAltitude;

	public MSC(String _MscID, String _MscName, String _MscCompany, String _MscLongitude, 
			String _MscLatitude, String _MscAltitude){
		this._MscID = _MscID;
		this._MscName = _MscName;
		this._MscCompany = _MscCompany;
		this._MscLongitude = _MscLongitude;
		this._MscLatitude = _MscLatitude;
		this._MscAltitude = _MscAltitude;
		
		this.MscID = _MscID;
		this.MscName = _MscName;
		this.MscCompany = _MscCompany;
		this.MscLongitude = 0;
		this.MscLatitude = 0;
		this.MscAltitude = 0;
	}	

	public boolean checkRow(){
		try{
			MscLongitude = Float.parseFloat(_MscLongitude);
			MscLatitude = Float.parseFloat(_MscLatitude);
			MscAltitude = Integer.parseInt(_MscAltitude);
			if (MscLongitude < 0 || MscLatitude < 0 || MscAltitude < 0)
				return false;
			return true;
		} catch(NumberFormatException e){
			return false;
		}
	}
	
	public String getMscID() {
		return MscID;
	}

	public String getMscName() {
		return MscName;
	}

	public String getMscCompany() {
		return MscCompany;
	}

	public float getMscLongitude() {
		return MscLongitude;
	}

	public float getMscLatitude() {
		return MscLatitude;
	}

	public int getMscAltitude() {
		return MscAltitude;
	}
}
