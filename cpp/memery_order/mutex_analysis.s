set_values(double, int):                       # @set_values(double, int)
        push    rbx
        sub     rsp, 16
        mov     ebx, edi
        movsd   qword ptr [rsp + 8], xmm0 # 8-byte Spill
        mov     edi, offset mutex
        call    std::__1::mutex::lock()                       ## 先锁住
        movsd   xmm0, qword ptr [rsp + 8] # 8-byte Reload     ## dvalue = dv;
        movsd   qword ptr [rip + dvalue], xmm0                ## ivalue = iv;
        mov     dword ptr [rip + ivalue], ebx
        mov     edi, offset mutex
        call    std::__1::mutex::unlock()                      ## 析构解锁
        mov     al, 1
        add     rsp, 16
        pop     rbx
        ret