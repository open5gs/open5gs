import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import SearchIcon from 'react-icons/lib/md/search';
import ClearIcon from 'react-icons/lib/md/clear';

const Wrapper = styled.div`
  display: flex;
  align-items: center;

  width: 700px;
  margin: 2rem auto 1rem auto;

  background: white;
  color: ${oc.gray[6]};
  box-shadow: 0 1px 3px rgba(0,0,0,0.12), 0 1px 2px rgba(0,0,0,0.24);
  transition: all 0.3s cubic-bezier(.25,.8,.25,1);
      
  &:hover {
    box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23);
  }

  ${media.tablet`
    width: 400px;
  `}

  ${media.mobile`
    margin: 0rem auto;
    width: 100%;
  `}
`;

const SearchIconWrapper = styled.div`
  display: inline-flex;
  margin-left: 1rem;
  font-size: 1.5rem;
`

const Input = styled.input`
  padding : 1rem;
  margin: 0 auto;
  width: 100%;

  font-size: 1.5rem;

  cursor: text;

  border: none;
  outline: none;
`
const ClearIconWrapper = styled.div`
  display: inline-flex;
  margin-right: 1rem;
  font-size: 1.5rem;

  cursor: pointer;
`

const Search = ({ value, onChange, onClear }) => (
  <Wrapper>
    <SearchIconWrapper><SearchIcon/></SearchIconWrapper>
    <Input 
      value={value}
      onChange={onChange}/>
    {value !== '' && 
      <ClearIconWrapper onClick={onClear}>
        <ClearIcon/>
      </ClearIconWrapper>
    }
  </Wrapper>

)

Search.propTypes = {
  value: PropTypes.string,
  onChange: PropTypes.func,
  onClear: PropTypes.func
};

export default Search;