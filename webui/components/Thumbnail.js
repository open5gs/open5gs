import PropTypes from 'prop-types';

import styled from 'styled-components';
import Person from 'react-icons/lib/md/person';
import oc from 'open-color';

const Wrapper = styled.div`
  // Layout
  width: ${props => props.size};
  height: ${props => props.size};
  display: flex;
  align-items: center;
  justify-content: center;

  // Misc
  border-radius: calc(${props => props.size} * 0.5 ); // to be circle, this value is more than 1/2
  font-size: calc(${props => props.size} * 0.75);

  background: ${props => props.color};
  color: white;

`;

Wrapper.propTypes = {
  size: PropTypes.string,
  color: PropTypes.string
};

const Thumbnail = ({size, color}) => (
  <Wrapper size={size} color={color}>
      <Person />
  </Wrapper>
)

Thumbnail.propTypes = {
  size: PropTypes.string,
  color: PropTypes.string
};

Thumbnail.defaultProps = {
  size: '4rem',
  color: '#000'
};

export default Thumbnail;