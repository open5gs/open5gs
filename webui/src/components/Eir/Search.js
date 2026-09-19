import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';
import SearchIcon from 'react-icons/lib/md/search';
import ClearIcon from 'react-icons/lib/md/clear';
const Wrapper = styled.div`
  display: flex;
  align-items: center;
  width: 700px;
  margin: 2rem auto 1rem;
  background: white;
  color: ${oc.gray[6]};
  box-shadow: 0 1px 3px rgba(0, 0, 0, 0.12), 0 1px 2px rgba(0, 0, 0, 0.24);
  transition: all 0.3s cubic-bezier(0.25, 0.8, 0.25, 1);
  &:hover {
    box-shadow: 0 10px 20px rgba(0, 0, 0, 0.19), 0 6px 6px rgba(0, 0, 0, 0.23);
  }
  ${media.tablet`width:400px;`}${media.mobile`margin:0 auto;width:100%;`}
`;
const Input = styled.input`
  padding: 1rem;
  margin: 0 auto;
  width: 100%;
  font-size: 1.5rem;
  cursor: text;
  border: none;
  outline: none;
`;
const Icon = styled.div`
  display: inline-flex;
  margin-left: 1rem;
  font-size: 1.5rem;
`;
const Clear = styled.div`
  display: inline-flex;
  margin-right: 1rem;
  font-size: 1.5rem;
  cursor: pointer;
`;
export default ({ value, onChange, onClear }) => (
  <Wrapper>
    <Icon>
      <SearchIcon />
    </Icon>
    <Input value={value} onChange={onChange} />
    {value !== '' && (
      <Clear onClick={onClear}>
        <ClearIcon />
      </Clear>
    )}
  </Wrapper>
);
