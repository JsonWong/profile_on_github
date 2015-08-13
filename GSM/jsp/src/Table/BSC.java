package Table;

public class BSC {
	private String _BscId;
	private String _BscName;
	private String _BscCompany;
	private String _Longitude;
	private String _Latitude;
	private String _MscID;
	
	private String BscId;
	private String BscName;
	private String BscCompany;
	private float Longitude;
	private float Latitude;
	private String MscID;	

	public BSC(String _BscId, String _BscName, String _BscCompany, String _Longitude, 
			String _Latitude, String _MscID){
		this._BscId = _BscId;
		this._BscName = _BscName;
		this._BscCompany = _BscCompany;
		this._Longitude = _Longitude;
		this._Latitude = _Latitude;
		this._MscID = _MscID;
		
		this.BscId = _BscId;
		this.BscName = _BscName;
		this.BscCompany = _BscCompany;
		this.Longitude = 0;
		this.Latitude = 0;
		this.MscID = _MscID;
	}
	
	public boolean checkRow(){
		try{
			Longitude = Float.parseFloat(_Longitude);
			Latitude = Float.parseFloat(_Latitude);
			if (Longitude < 0 || Latitude < 0)
				return false;
			return true;
		} catch(NumberFormatException e){
			return false;
		}
	}
	
	public String getBscId() {
		return BscId;
	}

	public String getBscName() {
		return BscName;
	}

	public String getBscCompany() {
		return BscCompany;
	}

	public float getLongitude() {
		return Longitude;
	}

	public float getLatitude() {
		return Latitude;
	}

	public String getMscID() {
		return MscID;
	}
}
