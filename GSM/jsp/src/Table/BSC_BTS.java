package Table;

public class BSC_BTS {
	private String _BscID;
	private String _BtsName;
	
	private String BscID;
	private String BtsName;	

	public BSC_BTS(String _BscID, String _BtsName){
		this._BscID = _BscID;
		this._BtsName = _BtsName;
		
		this.BscID = _BtsName;
		this.BtsName = _BtsName;
	}
	
	public boolean checkRow(){
		try{
			if (Integer.parseInt(BscID) < 0 || BtsName == "")
				return false;
			return true;
		} catch(NumberFormatException e){
			return false;
		}
	}

	public String getBscID() {
		return BscID;
	}

	public String getBtsName() {
		return BtsName;
	}
}
