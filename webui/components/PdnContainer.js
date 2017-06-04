import styled from 'styled-components';
import oc from 'open-color';

const Wrapper = styled.div`
  display: flex;
  align-items: center;
  justify-content: center;

  font-size: 4rem;
  line-height: 8rem;
  color: ${oc.gray[5]};

  border-top: 1px solid ${oc.gray[4]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.10), 3px 3px 6px rgba(0,0,0,0.20);

  background-color: white;
`;

const PdnContainer = () => (
  <Wrapper>
    PDN
  </Wrapper>
);

export default PdnContainer;