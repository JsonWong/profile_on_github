package Table;

public class Road_data {	
	private String _KeyNum;
	private String _CellID;
	private String _Latitude;
	private String _Longitude;
	private String _RxLev;
	
	private String KeyNum;
	private int CellID;
	private float Latitude;
	private float Longitude;
	private float RxLev;

	public Road_data(String _KeyNum, String _CellID, String _Latitude, String _Longitude, String _RxLev){
		this._KeyNum = _KeyNum;
		this._CellID = _CellID;
		this._Latitude = _Latitude;
		this._Longitude = _Longitude;
		this._RxLev = _RxLev;
		
		this.KeyNum = _KeyNum;
		this.CellID = 0;
		this.Latitude = 0;
		this.Longitude = 0;
		this.RxLev = 0;
	}	

	public boolean checkRow(){
		try{
			CellID = Integer.parseInt(_CellID);
			Latitude = Float.parseFloat(_Latitude);
			Longitude = Float.parseFloat(_Longitude);
			RxLev = Float.parseFloat(_RxLev);
			if (CellID < 0 || Latitude < 0 || Longitude < 0)
				return false;
			return true;
		} catch(NumberFormatException e){
			return false;
		}
	}

	public String getKeyNum() {
		return KeyNum;
	}

	public int getCellID() {
		return CellID;
	}

	public float getLatitude() {
		return Latitude;
	}

	public float getLongitude() {
		return Longitude;
	}

	public float getRxLev() {
		return RxLev;
	}
}
