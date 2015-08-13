package Table;

public class Cell {	
	private String _CellID;
	private String _BtsName;
	private String _AreaName;
	private String _Lac;
	private String _Longitude;
	private String _Latitude;
	private String _Direction;
	private String _Bcch;
	
	private int CellID;
	private String BtsName;
	private String AreaName;
	private int Lac;
	private float Longitude;
	private float Latitude;
	private int Direction;
	private int Bcch;

	public Cell(String _BtsName, String _BscId, String _Longitude, String _Latitude, 
			String _Altitude, String _BtsCompany, String _BtsPower, String _Bcch){
		this._CellID = _CellID;
		this._BtsName = _BtsName;
		this._AreaName = _AreaName;
		this._Lac = _Lac;
		this._Longitude = _Longitude;
		this._Latitude = _Latitude;
		this._Direction = _Direction;
		this._Bcch = _Bcch;
		
		this.CellID = 0;
		this.BtsName = _BtsName;
		this.AreaName = _AreaName;
		this.Lac = 0;
		this.Longitude = 0;
		this.Latitude = 0;
		this.Direction = 0;
		this.Bcch = 0;
	}	

	public boolean checkRow(){
		try{
			Longitude = Float.parseFloat(_Longitude);
			Latitude = Float.parseFloat(_Latitude);
			if (CellID < 0 || Lac < 0 || Longitude < 0)
				return false;
			return true;
		} catch(NumberFormatException e){
			return false;
		}
	}
	
	public int getCellID() {
		return CellID;
	}

	public String getBtsName() {
		return BtsName;
	}

	public String getAreaName() {
		return AreaName;
	}

	public int getLac() {
		return Lac;
	}

	public float getLongitude() {
		return Longitude;
	}

	public float getLatitude() {
		return Latitude;
	}

	public int getDirection() {
		return Direction;
	}

	public int getBcch() {
		return Bcch;
	}
}
