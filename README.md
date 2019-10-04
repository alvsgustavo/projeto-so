# Laboratório 3: Escalonamento

### Escalonamento probabilístico

No escalonamento probabilístico, foi-se utilizado a ideia de _Lottery Scheduling_ para o escalonador. Este escalonador funciona assim: Um número de tíquetes é distribuído aos processos de acordo com a sua prioridade. Após isto, sorteia-se um número (tíquete) entre 0 e o número total de tíquetes distribuídos. O processo que tiver o tíquete com número igual ao sorteado é o que será escalonado.

### Escalonamento determinístico

No escalonamento determinístico, os processos com maiores prioridades serão os primeiros a executarem. Para evitar que alguns processos fiquem esfomeados (_starvation_) uma função `starvationAvoidance` é responsável por aumentar a prioridade de processos para que em algum momento sejam escalonados. Neste escalonamento, processos com prioridades iguais são organizados ao estilo round robin.

Em ambos os escalonamentos, processos que são estão com o estado `SLEEPING` tem sua prioridade aumentada, pois estão sempre passando a sua vez enquanto dormem.
