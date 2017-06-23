import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import SchoolIcon from 'react-icons/lib/md/school'

const Wrapper = styled.div`
  position: relative;
  display: block;

  margin-top : 6rem;
`  

const CircleIcon = styled.div`
  display: flex;
  align-items: center;
  justify-content: center;

  width: ${props => props.size};
  height: ${props => props.size};
  margin: 0 auto;

  border-radius: calc(${props => props.size} * 0.5 );
  font-size: calc(${props => props.size} * 0.75);

  background: ${oc.gray[0]};
  color: ${oc.gray[8]};
`;

const StyledTitle = styled.div`
  display: block;
  margin-top: 2rem;
  text-align: center;
  font-size: 1rem;
  font-weight: bold;

  color: ${oc.indigo[8]};
  cursor: pointer;
`;

const StyledBody = styled.div`
  display: block;
  margin-top: 2rem;
  text-align: center;
  font-size: 1rem;
  font-weight: 300;

  color: ${oc.gray[6]};
  cursor: default;
`;

const Blank = ({visible, title, body, onTitle}) => visible ? (
  <Wrapper>
    <CircleIcon size="12rem">
      <SchoolIcon/>
    </CircleIcon>
    <StyledBody>{body}</StyledBody>
    <StyledTitle onClick={onTitle}>{title}</StyledTitle>
  </Wrapper>
) : null;

Blank.propTypes = {
  visible: PropTypes.bool,
  title: PropTypes.string,
  body: PropTypes.string,
  onTitle: PropTypes.func
};

Blank.defaultProps = {
  visible: false,
  title: "ADD A DATA",
  body: "You have no data... yet!"
};

export default Blank;