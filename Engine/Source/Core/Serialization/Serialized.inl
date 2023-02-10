/*// -----------------------------------------------------------------
*  File:		Serialized.inl
*  Brief:		Templated helpers for serialization
*  Creation:	10/02/2023
*  Last Update:	10/02/2023
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/

/* // ---------------------------------------------------------------------
*	Brief:		Copies vector of any dimension and any type to json
*/ // ---------------------------------------------------------------------
template<int vecDim, typename Type>
nlohmann::ordered_json& operator<<(nlohmann::ordered_json& data, const glm::vec<vecDim, Type>& vec) noexcept
{
	//Copy data to json and return
	for (int i = 0; i < vecDim; i++)
		data.push_back(vec[i]);
	return data;
}

/* // ---------------------------------------------------------------------
*	Brief:		Copies json to vector of any dimension and any data
*/ // ---------------------------------------------------------------------
template<int vecDim, typename Type>
glm::vec<vecDim, Type>& operator>>(const nlohmann::ordered_json& data, glm::vec<vecDim, Type>& vec) noexcept
{
	//Check that the json satisfies vector data before copying
	bool correct = data.is_array() && data.size() == vecDim;

	//If satisfied, copy to vector and return it
	for (int i = 0; i < vecDim; i++)
		correct ? vec[i] = data[i] : vec[i] = 0;
	return vec;

}