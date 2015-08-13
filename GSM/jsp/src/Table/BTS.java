package Table;

public class BTS {
	private String _BtsName;
	private String _BscId;
	private String _Longitude;
	private String _Latitude;
	private String _Altitude;
	private String _BtsCompany;
	private String _BtsPower;
	
	private String BtsName;
	private String BscId;
	private float Longitude;
	private float Latitude;
	private int Altitude;
	private String BtsCompany;
	private int BtsPower;

	public BTS(String _BtsName, String _BscId, String _Longitude, String _Latitude, 
			String _Altitude, String _BtsCompany, String _BtsPower){
		this._BtsName = _BtsName;
		this._BscId = _BscId;
		this._Longitude = _Longitude;
		this._Latitude = _Latitude;
		this._Altitude = _Altitude;
		this._BtsCompany = _BtsCompany;
		this._BtsPower = _BtsPower;
		
		this.BtsName = _BtsName;
		this.BscId = _BscId;
		this.Longitude = 0;
		this.Latitude = 0;
		this.Altitude = 0;
		this.BtsCompany = _BtsCompany;
		this.BtsPower = 0;
	}	

	public boolean checkRow(){
		try{
			Longitude = Float.parseFloat(_Longitude);
			Latitude = Float.parseFloat(_Latitude);
			if (Longitude < 0 || Latitude < 0 || BtsPower < 0)
				return false;
			return true;
		} catch(NumberFormatException e){
			return false;
		}
	}
	
	public String getBtsName() {
		return BtsName;
	}

	public String getBscId() {
		return BscId;
	}

	public float getLongitude() {
		return Longitude;
	}

	public float getLatitude() {
		return Latitude;
	}

	public int getAltitude() {
		return Altitude;
	}

	public String getBtsCompany() {
		return BtsCompany;
	}

	public int getBtsPower() {
		return BtsPower;
	}
}
