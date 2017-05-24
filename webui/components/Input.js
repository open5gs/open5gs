import styled from 'styled-components';
import oc from 'open-color';
import PropTypes from 'prop-types';

const Input = styled.input`
  // Layout
  width: 100%;
  padding: 0.5rem;

  // Color
  border: 1px solid ${oc.gray[2]};

  // Misc
  font-size: 1.5rem;
  line-height: 2rem;
  transition: all .25s;

  // During keyboard input
  &:focus {
    outline: none;
    border: 1px solid ${oc.pink[3]};
    color: ${oc.pink[6]};
  }

  // Interval Between Component
  & + & {
    margin-top: 1rem;
  }
`;

Input.propTypes = {
  name: PropTypes.string,
  value: PropTypes.string,
  placeholder: PropTypes.string,
  onChange: PropTypes.func
};

export default Input