import styled from 'styled-components';
import oc from 'open-color';

const Wrapper = styled.div`
  position: absolute;
  margin: 0 auto;
  width: 100%;

  background: ${oc.indigo[6]};
  color: white;
  border-bottom: 1px solid ${oc.indigo[7]};
  box-shadow: 0 3px 6px rgba(0,0,0,0.10), 0 3px 6px rgba(0,0,0,0.20);

  font-size: 2.5rem;
`;

export default () => (
  <Wrapper>
     NextEPC
 </Wrapper>
)
